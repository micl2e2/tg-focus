#ifndef _PRODUCER_H
#define _PRODUCER_H

#include <chrono>
#include <iterator>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <fmt/core.h>
#include <iostream>

#include "td_client.hh"
#include "focusd_state.hh"

void
focusd_producer ()
{
  while (true)
    {
      is_csm_mq.wait (true, std::memory_order_acquire);

      std::cerr << fmt::format ("[PRODUCER {}] mq size: {}",
				it_cnt_producer.load (
				  std::memory_order_relaxed),
				mq.size ())
		<< std::endl;

      td_client.fetch_updates ();

      it_cnt_producer.fetch_add (1, std::memory_order_relaxed);
    }
}

#endif
