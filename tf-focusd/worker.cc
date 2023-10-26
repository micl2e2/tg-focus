#include <mutex>
#include <atomic>
#include <thread>
#include <fmt/core.h>
#include <iostream>

#include "lv_log.hh"
#include "state.hh"
#include "focus_filter.hh"
#include "tf_msg.hh"

void
focusd_producer ()
{
  while (true)
    {
      is_csm_mq.wait (true, std::memory_order_acquire);

      lv_log (LogLv::DEBUG, "producer_iter:{}, mq size: {}",
	      it_cnt_producer.load (std::memory_order_relaxed), mq.size ());

      collector.fetch_updates ();

      it_cnt_producer.fetch_add (1, std::memory_order_relaxed);
    }
}

bool
need_collect (const TgMsg &msg)
{
  auto tomlstr = tf_data.get_filters ();
  lv_log (LogLv::INFO, "consumer_iter:{}, filters reloaded",
	  it_cnt_consumer.load (std::memory_order_relaxed));
  auto fcf_list = FocusFilterList (tomlstr);
  if (fcf_list.is_tgmsg_match (msg))
    return true;

  return false;
}

void
focusd_consumer ()
{
  using namespace std;
  int consume_cnt = 0;
  while (1)
    {
      // std::this_thread::sleep_for (std::chrono::seconds (1));

      is_csm_mq.wait (false, std::memory_order_acquire);

      if (!collector.tried_create_collector)
	{
	  collector.create_tgfocus_group ();
	  continue;
	}

      // try consume mq
      {
	std::lock_guard<std::mutex> mq_guard (mq_lock);

	if (mq.size () > 0 && collector.done_create_collector)
	  {
	    lv_log (LogLv::DEBUG,
		    "consumer_iter:{}, mq consumable, mq.size():{} ",
		    it_cnt_consumer.load (std::memory_order_relaxed),
		    mq.size ());

	    for (auto it = mq.begin (); it != mq.end (); it += 1)
	      {
		auto curr_msg = *it;
		if (!curr_msg.is_from_tgfocus ())
		  {
		    // FIXME: too many disk io incurred by need_collect
		    if (need_collect (curr_msg))
		      {
			collector.collect_msg (std::move (curr_msg),
					       consume_cnt);
			consume_cnt++;
		      }
		    else
		      lv_log (LogLv::DEBUG,
			      "consume cnt:{} message not collected: {}",
			      it_cnt_consumer.load (std::memory_order_relaxed),
			      curr_msg.to_string ());
		  }
	      }

	    {
	      auto tmp = std::move (mq);
	    }
	  }
	else
	  {
	    lv_log (LogLv::DEBUG,
		    "consumer_iter:{}, mq not consumable, mq.size():{} ",
		    it_cnt_consumer.load (std::memory_order_relaxed),
		    mq.size ());
	  }
      }

      it_cnt_consumer.fetch_add (1, std::memory_order_relaxed);
      is_csm_mq.store (false, std::memory_order_release);
      is_csm_mq.notify_all ();
    }
}

void
focusd_switcher ()
{
  while (true)
    {
      std::this_thread::sleep_for (std::chrono::seconds (5));

      {
	lv_log (LogLv::INFO,
		"switcher summary, P,{} C,{} S,{} mqsize:{} "
		"nhandle:{},nuser:{},nchattitle:{}  ",
		it_cnt_producer.load (std::memory_order_relaxed),
		it_cnt_consumer.load (std::memory_order_relaxed),
		it_cnt_switcher.load (std::memory_order_relaxed), mq.size (),
		collector.n_handlers (), collector.n_users (),
		collector.n_chat_titles ());
      }

      if (is_csm_mq.load (std::memory_order_acquire))
	{
	  lv_log (LogLv::INFO,
		  "switcher cnt:{}, has msg, consumer maybe handling...",
		  it_cnt_switcher.load (std::memory_order_relaxed));
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
    }
}
