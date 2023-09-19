#ifndef _SWITCHER_H
#define _SWITCHER_H

#include <cstdint>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>

#include "tgfocus_state.hh"

extern std::vector<TgMsg> mq;
extern std::mutex mq_lock;
extern std::atomic<bool> is_csm_mq;
extern TdClient td_client;
extern std::atomic<std::uint32_t> it_cnt_switcher;

void
tgmsg_switcher ()
{
  // size_t it_count = 0;
  while (true)
    {
      std::this_thread::sleep_for (std::chrono::seconds (5));

      {
	std::cout
	  << std::format ("[SWITCHER {}] P,{} C,{} S,{} check for switching...",
			  it_cnt_switcher.load (std::memory_order_relaxed),
			  it_cnt_producer.load (std::memory_order_relaxed),
			  it_cnt_consumer.load (std::memory_order_relaxed),
			  it_cnt_switcher.load (std::memory_order_relaxed))

	  << std::endl;
      }

      if (is_csm_mq.load (std::memory_order_acquire))
	{
	  std::cout
	    << std::format ("[SWITCHER {}] has msg, consumer maybe handling...",
			    it_cnt_switcher.load (std::memory_order_relaxed))
	    << std::endl;
	  continue;
	}

      {
	std::lock_guard<std::mutex> mq_guard (mq_lock);
	if (mq.size () > 0)
	  {
	    is_csm_mq.store (true, std::memory_order_release); // unlock
	    is_csm_mq.notify_all ();
	  }
      }

      it_cnt_switcher.fetch_add (1, std::memory_order_relaxed);
      // it_count++;
    }
}

#endif
