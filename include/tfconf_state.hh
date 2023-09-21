#ifndef _TFCONF_STATE_H
#define _TFCONF_STATE_H

#include <atomic>
#include <fmt/core.h>
#include <string_view>
#include <iostream>
#include "tf_data.hh"

extern std::atomic<bool> is_login;
extern std::atomic<bool> is_tdlib_closed;
extern TgFocusData tf_data;

template <class... Args>
void
log (fmt::format_string<Args...> fmt, Args &&...args)
{
  constexpr std::string_view header{"[tf-conf] "};
  std::cout << header << fmt::format (fmt, args...) << std::endl;
}

template <class... Args>
void
log_flush (fmt::format_string<Args...> fmt, Args &&...args)
{
  constexpr std::string_view header{"[tf-conf] "};
  std::cout << header << fmt::format (fmt, args...) << std::flush;
}

#endif
