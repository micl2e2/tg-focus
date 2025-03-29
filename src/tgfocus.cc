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
  printf (R"(TG-Focus

Usage: %s [command]

Available Commands:
  auth              Log in Telegram account
  auth-reset        Log out Telegram account
  quickstart        Start up everyhing
  shutdown          Shutdown everyhing
  startup           Start up the daemon
  focus-start       Start up the message collector (resume)
  focus-stop        Stop the message collector (pause)
  auth-cust-api     Log in Telegram (with provided API ID and API HASH)
  filters           Customize focus filter(s)
  use-chat          Initialize TG-FOCUS chat as a basic group (default)
  use-channel       Initialize TG-FOCUS chat as a super group
  lang <LANG_CODE>  Program-wide language preference. tg-focus will try to
                    use the language as long as supported by the host OS.
  version           Print version information
  help              Print this message
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
	  cout << "error: "
	       << "initialization failed" << endl;
	  cout << "(HINT: "
	       << "run `tgfocus startup` to initialize)" << endl;
	  break;
	default:
	  cout << "error: "
	       << "likely a bug, errcode:" << ec << endl;
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

  cout << ("Reset successfully") << endl;

  return 0;
}

int
handle_filters ()
{
  if (!tgfstat::userdata.prepare_filters_tmp ())
    return 2;

  auto fpath = tgfstat::userdata.path_filters_tmp ();
  auto fpath_cstr = fpath.c_str ();

  ostringstream runcmd;
  // string runcmd = "";
  const char *editor = getenv ("EDITOR");
  if (editor != nullptr)
    {
      runcmd << editor << " " << fpath_cstr;
    }
  else
    {
      if (false)
	;
      else if (fs::exists ("/usr/bin/mg"))
	{
	  runcmd << "/usr/bin/mg"
		 << " " << fpath_cstr;
	}
      else if (fs::exists ("/usr/bin/emacs"))
	{
	  runcmd << "/usr/bin/emacs"
		 << " "
		 << "-Q"
		 << " " << fpath_cstr;
	}
      else if (fs::exists ("/usr/bin/nano"))
	{
	  runcmd << "/usr/bin/nano"
		 << " " << fpath_cstr;
	}
      else if (fs::exists ("/usr/bin/vi"))
	{
	  runcmd << "/usr/bin/vi"
		 << " " << fpath_cstr;
	}
    }

  if (runcmd.str ().length () < 10)
    {
      cerr << "error: no valid editors" << endl;
      return 255;
    }

  std::system (runcmd.str ().c_str ());

  cout << ("Verifying...");

  toml::value res;

  try
    {
      res = toml::parse (fpath_cstr);
      cout << "good" << endl;
    }
  catch (const std::exception &ex)
    {
      cout << "bad"
	   << "(invalid data)" << endl;
      return 255;
    }

  FileReader reader{fpath_cstr};
  auto filterg = tgf::FilterGroupToml (reader.read_to_string ().value_or ("-"));
  cout << ("Saving...");
  if (filterg.isEffective ())
    {
      tgfstat::userdata.set_filters (tgfstat::userdata.get_filters_tmp ());
      cout << ("good") << endl;
    }
  else
    {
      cout << ("bad") << "(invalid filters)" << endl;
      return 255;
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
  cout << ("setting up language...", is_good ? "done" : "failed") << endl;

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
handle_startup_FORK ()
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
      // !!!BLOCKING
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
handle_quickstart ()
{
  int ret = 1;

  ret = handle_startup_FORK ();
  if (!tgfstat::im_child.load (mo::relaxed) && ret == 0)
    {
      this_thread::sleep_for (chrono::seconds (1));
      ret = handle_upcoll ();
    }
  return ret;
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
      cout << s;
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

  const char *subcmd = "";

  // handle options
  for (int i = 1; i < argc; i++)
    {
      if (argv[i][0] == '-')
	{
	  if (strcmp (argv[i], "--verbose") == 0)
	    {
	      tgf::g_loglv = tgf::LogLv::DEBUG;
	    }
	}
      else
	{
	  subcmd = argv[i];
	}
    }

  if (strlen (subcmd) < 1)
    return print_usage (argv);

  if (strcmp (subcmd, "startup") == 0)
    return handle_startup_FORK ();

  if (strcmp (subcmd, "shutdown") == 0)
    return handle_shutipcsrv ();

  if (strcmp (subcmd, "focus-start") == 0)
    return handle_upcoll ();

  if (strcmp (subcmd, "focus-stop") == 0)
    return handle_shutcoll ();

  if (strcmp (subcmd, "quickstart") == 0)
    return handle_quickstart ();

  if (strcmp (subcmd, "status") == 0)
    return handle_rstatus ();

  if (strcmp (subcmd, "auth-reset") == 0)
    return handle_auth_reset ();

  if (strcmp (subcmd, "auth") == 0)
    return handle_auth (true);

  if (strcmp (subcmd, "auth-cust-api") == 0)
    return handle_auth (false);

  if (strcmp (subcmd, "filters") == 0)
    return handle_filters ();

  if (strcmp (subcmd, "version") == 0 || strcmp (subcmd, "--version") == 0
      || strcmp (subcmd, "-v") == 0)
    return handle_version ();

  if (strcmp (subcmd, "lang") == 0)
    {
      if (argc != 3)
	{
	  print_usage (argv);
	  return 0;
	}
      return handle_lang (argv[2]);
    }

  if (strcmp (subcmd, "use-chat") == 0)
    {
      return handle_super_tgfid (1);
    }

  if (strcmp (subcmd, "use-channel") == 0)
    {
      return handle_super_tgfid (2);
    }

  return print_usage (argv);
}
