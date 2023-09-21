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
#include <fmt/core.h>
#include <string_view>
#include <thread>
#include <unistd.h>
#include <vector>

#include "common.hh"
#include "toml.hpp"

#include "tfconf_state.hh"

#include "tfconf_auth.hh"
#include "focus_filter.hh"

int
print_usage (char *argv[])
{
  printf ("Usage: %s auth/filters\n", argv[0]);
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

  log ("reset successfully");

  return 0;
}

int
handle_filters ()
{
  if (!tf_data.prepare_filters_tmp ())
    return 2;

  auto fpath = tf_data.path_filters_tmp ();
  auto fpath_cstr = fpath.c_str ();

  // std::system (fmt::format ("$EDITOR {}", fpath_cstr).c_str ());
  std::system (fmt::format ("nano {}", fpath_cstr).c_str ());

  log ("Verifying filters...");

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
    log ("ERROR: Invalid toml");
  else
    {
      FileReader reader{fpath_cstr};
      if (FocusFilterList::is_valid (reader.read_to_string ().value_or ("-")))
	{
	  log ("Saving filters...");
	  tf_data.set_filters (tf_data.get_filters_tmp ());
	}
      else
	log ("ERROR: Invalid filters");
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  if (argc != 2)
    return print_usage (argv);

  std::string subcmd = argv[1];

  if (subcmd == "auth-reset")
    return handle_auth_reset ();

  if (subcmd == "auth")
    return handle_auth ();

  if (subcmd == "filters")
    return handle_filters ();

  return print_usage (argv);
}
