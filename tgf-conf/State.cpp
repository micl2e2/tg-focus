#include "State.hpp"

std::atomic<bool> is_login{false};
std::atomic<bool> is_tdlib_closed{false};

TgfData tgf_data{std::nullopt, false};
