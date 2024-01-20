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
#include "tf_locale.hh"

int
print_usage (char *argv[])
{
  printf (R"(Configure TG-Focus

Usage: %s [command]

Available Commands:
  auth           Log in Telegram
  auth-reset     Log out Telegram
  filters        Customize focus filter(s)
  version        Print TG-Focus version
  help           Print this message
)",
	  argv[0]);
  return 0;
}

int
handle_version ()
{
  printf (R"(TG-Focus:     %s
Default Chat: %s
Device ID:    %s
TDLib:        %s(%s)

Copyright (C) 2023 Michael Lee <micl2e2@proton.me>
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
)",
	  TF_VER, TF_COLL_CHAT_TITLE, TF_DEV, TDLIB_VER, TDLIB_SRC);

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
  tf_data.set_auth_hint (false);

  // second
  auto del_res = std::filesystem::remove_all (tf_data.path_tddata (), ec);
  if (del_res == static_cast<std::uintmax_t> (-1))
    return 1;

  lvlog (LogLv::INFO, "Reset successfully");

  return 0;
}

int
handle_filters ()
{
  if (!tf_data.prepare_filters_tmp ())
    return 2;

  auto fpath = tf_data.path_filters_tmp ();
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
	  tf_data.set_filters (tf_data.get_filters_tmp ());
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
main (int argc, char *argv[])
{
  if (!tgf::try_ensure_locale ())
    {
      lvlog (LogLv::WARNING, "Available locales not found");
    }

  if (argc != 2)
    return print_usage (argv);
  handle_loglv (argc, argv);

  std::string subcmd = argv[1];

  if (subcmd == "auth-reset")
    return handle_auth_reset ();

  if (subcmd == "auth")
    return handle_auth ();

  if (subcmd == "filters")
    return handle_filters ();

  if (subcmd == "version" || subcmd == "--version" || subcmd == "-v")
    return handle_version ();

  return print_usage (argv);
}
