#include "State.hpp"

std::atomic<std::uint32_t> it_cnt_switcher;
std::atomic<std::uint32_t> it_cnt_producer;
std::atomic<std::uint32_t> it_cnt_consumer;

std::vector<TgfMsg> mq;
std::mutex mq_lock;
std::atomic<bool> is_csm_mq;
std::atomic<bool> is_tdlib_auth;
std::atomic<bool> need_phone;
TdCollector collector;
TgfData tgf_data{std::nullopt, false};
std::atomic<int> producer_hang_period{0};
