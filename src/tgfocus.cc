#define __TU__ "tgfocus"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <vector>

#include "toml.hpp"
#include "std_comp.hh"
#include "errcode.hh"
#include "log.hh"
#include "common.hh"
#include "stat.hh"
#include "td_auth.hh"
#include "filter.hh"
#include "locale_conf.hh"
#include "ipc_client.hh"
#include "ipc_msg.hh"
#include "worker/ipc_server_initer.hh"
#include "worker/ipc_msg_dequer.hh"
#include "worker/coll_initer.hh"

int
print_usage (char *argv[])
{
  printf (R"(Configure TG-Focus

Usage: %s [command]

Available Commands:
  auth              Log in Telegram
  auth-reset        Log out Telegram
  auth-cust-api     Log in Telegram (with API ID and API HASH customization)
  filters           Customize focus filter(s)
  use-chat          Initialize TG-FOCUS chat as a basic group (default)
  use-channel       Initialize TG-FOCUS chat as a super group
  version           Print TG-Focus version
  help              Print this message
  lang <LANG_CODE>  Program-wide language preference. tg-focus will try to
                    use the language specified, as long as it is supported
                    by system. The LANG_CODE conforms to "Language-Region"
                    definition in RFC5646.

                    For example, 'tf-conf lang en-US' indicates that 'English
                    (United States)' is in use. 'tf-conf lang en-GB' indicates
                    'English (United Kingdom)' is in use.

                    Currently supported languages:
                    - <LANG-CODE>: <LANGUAGE> (<REGION>)
                    - en-HK: English (Hong Kong)
                    - en-US: English (United States)
                    - en-ZW: English (Zimbabwe)
                    - zh-CN: Chinese (Mainland China)
                    - zh-HK: Chinese (Hong Kong)
)",
	  argv[0]);
  return 0;
}

bool
good_client_or_report (const tgf::IpcClient &client)
{
  tgf::ErrCode ec = client.ec ();
  if (ec > 0)
    {
      switch (ec)
	{
	case tgf::EC::IPCCLIENT_SYSCALL_CONNECT:
	case tgf::EC::IPCCLIENT_SYSCALL_CONNECT2:
	  cout << "error: " << "initialization failed" << endl;
	  cout << "(HINT: " << "run `tgfocus startup` to initialize)" << endl;
	  break;
	default:
	  cout << "error: " << "likely a bug, errcode:" << ec << endl;
	  break;
	}
      return false;
    }
  return true;
}

int
handle_version ()
{
  tgf::PREFER_LANG = tgfstat::userdata.get_pref_lang ();

  if (!tgf::try_ensure_locale ())
    tgf::logw ("Available utf8 locales not found");

  printf (R"(TG-Focus:     %s
Language:     %s
Default Chat: %s
Device ID:    %s
TDLib:        %s(%s)

Copyright (C) 2023-2024 Michael Lee <micl2e2@proton.me>
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
)",
	  TF_VER, tgf::lang_to_cstr (tgf::HOST_LANG), TF_COLL_CHAT_TITLE,
	  TF_DEV, TDLIB_VER, TDLIB_SRC);

  return 0;
}

int
handle_auth (bool useProvidedApiPass)
{
  tgf::TdAuth td_auth{useProvidedApiPass};
  td_auth.loop ();
  return 0;
}

int
handle_auth_reset ()
{
  std::error_code ec;

  // first
  tgfstat::userdata.set_auth_hint (false);

  // second
  auto del_res
    = std::filesystem::remove_all (tgfstat::userdata.path_tddata (), ec);
  if (del_res == static_cast<std::uintmax_t> (-1))
    return 1;

  // third
  // FIXME: tgfid cleanup

  tgf::logi ("Reset successfully");

  return 0;
}

int
handle_filters ()
{
  if (!tgfstat::userdata.prepare_filters_tmp ())
    return 2;

  auto fpath = tgfstat::userdata.path_filters_tmp ();
  auto fpath_cstr = fpath.c_str ();

  std::string runcmd = "$EDITOR ";
  runcmd += fpath_cstr;
  std::system (runcmd.c_str ());

  tgf::logi ("Verifying filters...");

  bool is_invalid_toml = false;
  toml::value res;

  try
    {
      res = toml::parse (fpath_cstr);
    }
  catch (const std::exception &ex)
    {
      is_invalid_toml = true;
    }

  if (is_invalid_toml)
    tgf::loge ("ERROR: Invalid toml");
  else
    {
      FileReader reader{fpath_cstr};
      auto filterg
	= tgf::FilterGroupToml (reader.read_to_string ().value_or ("-"));
      if (filterg.isEffective ())
	{
	  tgf::logi ("Saving filters...");
	  tgfstat::userdata.set_filters (tgfstat::userdata.get_filters_tmp ());
	}
      else
	tgf::loge ("ERROR: Invalid filters");
    }

  return 0;
}

void
handle_loglv (int argc, char *argv[])
{
  if (argc > 2)
    for (int i = 1; i < argc; i++)
      if (strcmp (argv[i], "--verbose") == 0)
	tgf::g_loglv = tgf::LogLv::DEBUG;
}

int
handle_lang (const char *lang_code)
{
  tgf::Lang l = tgf::lang_from_cstr (lang_code);
  tgfstat::userdata.set_pref_lang (l);
  tgf::PREFER_LANG = l;
  tgf::try_ensure_locale ();
  bool is_good = tgf::HOST_LANG == l;
  tgf::logi ("setting up language...", is_good ? "done" : "failed");

  return is_good ? 0 : 1;
}

int
handle_super_tgfid (uint8_t t)
{
  if (t == 2)
    tgfstat::userdata.set_super_tgfid ();
  if (t == 1)
    tgfstat::userdata.set_basic_tgfid ();
  return 0;
}

namespace childproc {

void
upAllWorkersOneShot ()
{
  {
    tgf::IpcServerInitializer worker = tgf::IpcServerInitializer ();
    std::jthread t (worker);
    tgf::ensure::is_valid_thread (t, tgf::EC::TGFOCUS_SYSCALL_THREAD);
    t.detach ();
  }

  {
    tgf::IpcMsgDequer worker = tgf::IpcMsgDequer ();
    std::jthread t (worker);
    tgf::ensure::is_valid_thread (t, tgf::EC::TGFOCUS_SYSCALL_THREAD2);
    t.detach ();
  }

  // ensure::daemonHandleIncomTgMsg (d);
  // ensure::daemonHandleFilteredTgMsg (d);
}

} // namespace childproc

int
handle_startup ()
{
  const string pubname = "startup: ";
  {
    tgf::IpcClient client = tgf::IpcClient ();
    if (client.active ())
      {
	cout << pubname << "already started" << endl;
	return 0;
      }
  }

  pid_t pid;

  if (signal (SIGCHLD, SIG_IGN) == SIG_ERR)
    {
      perror ("signal");
      exit (EXIT_FAILURE);
    }
  pid = fork ();
  switch (pid)
    {
    case -1:
      tulogfe (9991, strerror (errno));
      cout << pubname << "failed" << endl;
      exit (EXIT_FAILURE);
      break;
    case 0:
      tgfstat::im_child.store (true, mo::relaxed);
      childproc::upAllWorkersOneShot ();
      break;
    default:
      cout << pubname << "done" << endl;
      break;
    }
  // printf ("Child is PID %jd\n", (intmax_t) pid);

  if (pid == 0)
    {
      // tulogfi ("awaiting workers...");
      tulogfi (6661);
      tgfstat::c::tryshutwk::coll_initer_succ.wait (false, mo::relaxed);
      tulogfi (6662);
      tgfstat::c::tryshutwk::ipcsrv_initer_succ.wait (false, mo::relaxed);
      tulogfi (6663);
      tgfstat::c::tryshutwk::ipcconn_handler_succ.wait (false, mo::relaxed);
      tulogfi (6664);
      tgfstat::c::tryshutwk::ipcmsg_dequer_succ.wait (false, mo::relaxed);
      tulogfi (6665);

      tgfstat::c::tryshutwk::conn_listener_succ.wait (false, mo::relaxed);
      tulogfi (6666);

      tulogfi ("main shutdown V");
    }

  return 0;
}

int
handle_shutipcsrv ()
{
  const string pubname = "shutdown: ";
  tgf::IpcClient client = tgf::IpcClient ();
  if (!good_client_or_report (client))
    return 1;
  tgf::IpcMsg m = tgf::new_ipcmsg_shutipcsrv ();
  cout << pubname << "processing...(need 10~60s)" << endl;
  optional<tgf::IpcMsg> rsp = client.send_ipcmsg_sync (move (m));
  if (rsp && rsp->typ () == tgf::IpcMsgType::Success)
    {
      cout << pubname << "done" << endl;
      return 0;
    }
  else
    {
      cout << pubname << "failed" << endl;
      return 1;
    }

  return 1;
}

int
handle_upcoll ()
{
  const string pubname = "focus-start: ";
  tgf::IpcClient client = tgf::IpcClient ();
  if (!good_client_or_report (client))
    return 1;
  tgf::IpcMsg m = tgf::new_ipcmsg_upcoll ();
  std::optional<tgf::IpcMsg> rsp = client.send_ipcmsg_sync (std::move (m));
  if (rsp && rsp->typ () == tgf::IpcMsgType::Success)
    {
      cout << pubname << "done" << endl;
      return 0;
    }
  else
    {
      cout << pubname << "failed" << endl;
      return 1;
    }
}

int
handle_shutcoll ()
{
  const string pubname = "focus-stop: ";
  tgf::IpcClient client = tgf::IpcClient ();
  if (!good_client_or_report (client))
    return 1;
  tgf::IpcMsg m = tgf::new_ipcmsg_shutcoll ();
  cout << pubname << "processing...(need 10~60s)" << endl;
  std::optional<tgf::IpcMsg> rsp = client.send_ipcmsg_sync (std::move (m));
  if (rsp && rsp->typ () == tgf::IpcMsgType::Success)
    {
      cout << pubname << "done" << endl;
      return 0;
    }
  else
    {
      cout << pubname << "failed" << endl;
      return 1;
    }
}

string
u8vec_to_str (vector<u8> u8vec)
{
  string s = "";
  for (u8 ch : u8vec)
    {
      s += ch;
    }
  return s;
}

int
handle_rstatus ()
{
  const string pubname = "status: ";
  tgf::IpcClient client = tgf::IpcClient ();
  if (!good_client_or_report (client))
    return 1;
  tgf::IpcMsg m = tgf::new_ipcmsg_rstatus ();
  optional<tgf::IpcMsg> msg = client.send_ipcmsg_sync (move (m));
  if (msg && msg->typ () == tgf::IpcMsgType::SuccessReadable)
    {
      assert (msg->extargs ());
      char *bdata = rcast<char *> (msg->extargs ()->data ());
      string s (bdata, msg->extargs ()->size ());
      cout << s << endl;
      // cout << pubname << "done" << endl;
    }
  else
    {
      cout << pubname << "failed" << endl;
      return 1;
    }
  return 1;
}

int
main (int argc, char *argv[])
{
  if (!tgf::try_ensure_locale ())
    {
      tgf::logw ("Available locales not found");
    }

  if (argc < 2)
    return print_usage (argv);
  handle_loglv (argc, argv);

  std::string subcmd = argv[1]; // FIXME: use cstr

  if (subcmd == "startup")
    return handle_startup ();

  if (subcmd == "shutdown")
    return handle_shutipcsrv ();

  if (subcmd == "focus-start")
    return handle_upcoll ();

  if (subcmd == "focus-stop")
    return handle_shutcoll ();

  if (subcmd == "status")
    return handle_rstatus ();

  if (subcmd == "auth-reset")
    return handle_auth_reset ();

  if (subcmd == "auth")
    return handle_auth (true);

  if (subcmd == "auth-cust-api")
    return handle_auth (false);

  if (subcmd == "filters")
    return handle_filters ();

  if (subcmd == "version" || subcmd == "--version" || subcmd == "-v")
    return handle_version ();

  if (subcmd == "lang")
    {
      if (argc != 3)
	{
	  print_usage (argv);
	  return 0;
	}
      return handle_lang (argv[2]);
    }

  if (subcmd == "use-chat")
    {
      return handle_super_tgfid (1);
    }

  if (subcmd == "use-channel")
    {
      return handle_super_tgfid (2);
    }

  return print_usage (argv);
}
