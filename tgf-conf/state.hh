#ifndef _TFCONF_STATE_H
#define _TFCONF_STATE_H

#include <atomic>
#include <string_view>
#include <iostream>
#include "TgfData.hpp"

extern std::atomic<bool> is_login;
extern std::atomic<bool> is_tdlib_closed;
extern TgfData tgf_data;

#endif
