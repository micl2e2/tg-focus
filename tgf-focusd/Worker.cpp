#include <mutex>
#include <atomic>
#include <thread>
#include <iostream>

#include "LvLog.hpp"
#include "State.hpp"
#include "TgfFilter.hpp"
#include "TgfMsg.hpp"

void
focusd_producer ()
{}

bool
need_collect (const TgfMsg &msg)
{
  auto tomlstr = tgf_data.get_filters ();
  lvlog (LogLv::INFO,
	 "consumer_iter:", it_cnt_consumer.load (std::memory_order_relaxed),
	 " filters reloaded");

  auto filterg = TgfFilterGroupToml (tomlstr);
  if (filterg.isMatchTgfMsg (msg))
    return true;

  return false;
}

void
focusd_consumer ()
{
}

void
focusd_switcher ()
{
}
