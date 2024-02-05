#include <atomic>
#include <chrono>
#include <cstdint>
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
#include <vector>

#include "toml.hpp"
#include "common.hh"
#include "state.hh"
#include "auth.hh"
#include "focus_filter.hh"
#include "tgf_locale.hh"

int
print_usage (char *argv[])
{
  printf (R"(Configure TG-Focus

Usage: %s [command]

Available Commands:
  auth              Log in Telegram
  auth-reset        Log out Telegram
  filters           Customize focus filter(s)
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
                    - en_US: English (United States)
                    - en_GB: English (United Kingdom)
)",
	  argv[0]);
  return 0;
}

int
handle_version ()
{
  tgf::PREFER_LANG = tgf_data.get_pref_lang ();

  if (!tgf::try_ensure_locale ())
    lvlog (LogLv::WARNING, "Available utf8 locales not found");

  printf (R"(TG-Focus:     %s
Language:     %s
Default Chat: %s
Device ID:    %s
TDLib:        %s(%s)

Copyright (C) 2023 Michael Lee <micl2e2@proton.me>
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
)",
	  TF_VER, tgf::lang_to_cstr (tgf::HOST_LANG), TF_COLL_CHAT_TITLE,
	  TF_DEV, TDLIB_VER, TDLIB_SRC);

  return 0;
}

int
handle_auth ()
{
  TdAuth td_auth;
  td_auth.loop ();
  return 0;
}

int
handle_auth_reset ()
{
  std::error_code ec;

  // first
  tgf_data.set_auth_hint (false);

  // second
  auto del_res = std::filesystem::remove_all (tgf_data.path_tddata (), ec);
  if (del_res == static_cast<std::uintmax_t> (-1))
    return 1;

  lvlog (LogLv::INFO, "Reset successfully");

  return 0;
}

int
handle_filters ()
{
  if (!tgf_data.prepare_filters_tmp ())
    return 2;

  auto fpath = tgf_data.path_filters_tmp ();
  auto fpath_cstr = fpath.c_str ();

  std::string runcmd = "nano ";
  runcmd += fpath_cstr;
  std::system (runcmd.c_str ());

  lvlog (LogLv::INFO, "Verifying filters...");

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
    lvlog (LogLv::ERROR, "ERROR: Invalid toml");
  else
    {
      FileReader reader{fpath_cstr};
      if (FocusFilterList::is_valid (reader.read_to_string ().value_or ("-")))
	{
	  lvlog (LogLv::INFO, "Saving filters...");
	  tgf_data.set_filters (tgf_data.get_filters_tmp ());
	}
      else
	lvlog (LogLv::ERROR, "ERROR: Invalid filters");
    }

  return 0;
}

void
handle_loglv (int argc, char *argv[])
{
  if (argc > 2)
    for (int i = 1; i < argc; i++)
      if (strcmp (argv[i], "--verbose") == 0)
	g_log_lv = LogLv::DEBUG;
}

int
handle_lang (const char *lang_code)
{
  tgf::Lang l = tgf::lang_from_cstr (lang_code);
  tgf_data.set_pref_lang (l);
  tgf::PREFER_LANG = l;
  tgf::try_ensure_locale ();
  bool is_good = tgf::HOST_LANG == l;
  lvlog (LogLv::INFO, "setting up language...", is_good ? "done" : "failed");

  return is_good ? 0 : 1;
}

int
main (int argc, char *argv[])
{
  if (!tgf::try_ensure_locale ())
    {
      lvlog (LogLv::WARNING, "Available locales not found");
    }

  if (argc < 2)
    return print_usage (argv);
  handle_loglv (argc, argv);

  std::string subcmd = argv[1]; // FIXME: use cstr

  if (subcmd == "auth-reset")
    return handle_auth_reset ();

  if (subcmd == "auth")
    return handle_auth ();

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

  return print_usage (argv);
}
