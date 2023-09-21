#include "tfconf_state.hh"

std::atomic<bool> is_login{false};
std::atomic<bool> is_tdlib_closed{false};

TgFocusData tf_data{std::nullopt, false};
