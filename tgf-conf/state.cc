#include "state.hh"

std::atomic<bool> is_login{false};
std::atomic<bool> is_tdlib_closed{false};

TgFocusData tgf_data{std::nullopt, false};
