#ifndef _TFCONF_STATE_H
#define _TFCONF_STATE_H

#include <atomic>
#include <string_view>
#include <iostream>
#include "tgf_data.hh"

extern std::atomic<bool> is_login;
extern std::atomic<bool> is_tdlib_closed;
extern TgFocusData tgf_data;

#endif
