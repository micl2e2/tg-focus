#include <mutex>
#include <atomic>
#include <thread>
#include <iostream>

#include "lv_log.hh"
#include "state.hh"
#include "focus_filter.hh"
#include "tgf_msg.hh"

void
focusd_producer ()
{
  while (true)
    {
      is_csm_mq.wait (true, std::memory_order_acquire);

      lvlog (LogLv::DEBUG,
	     "producer_iter:", it_cnt_producer.load (std::memory_order_relaxed),
	     " mq size:", mq.size ());

      collector.fetch_updates ();

      it_cnt_producer.fetch_add (1, std::memory_order_relaxed);
    }
}

bool
need_collect (const TgMsg &msg)
{
  auto tomlstr = tgf_data.get_filters ();
  lvlog (LogLv::INFO,
	 " consumer_iter:", it_cnt_consumer.load (std::memory_order_relaxed),
	 " filters reloaded");

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

      if (!tgf_data.is_tgfid_valid () && !collector.tried_create_tgfchat)
	{
	  collector.try_create_tgfchat ();
	  continue;
	}

      // try consume mq
      {
	std::lock_guard<std::mutex> mq_guard (mq_lock);

	// if (mq.size () > 0 && collector.done_create_collector)
	if (mq.size () > 0 && tgf_data.is_tgfid_valid ())
	  {
	    lvlog (LogLv::DEBUG, "consumer_iter:",
		   it_cnt_consumer.load (std::memory_order_relaxed),
		   " mq consumable, mq.size():", mq.size ());

	    for (auto it = mq.begin (); it != mq.end (); it += 1)
	      {
		auto curr_msg = *it;
		// break; // stop collect
		if (!curr_msg.is_from_tgfocus ())
		  {
		    // FIXME: too many disk io incurred by need_collect
		    if (need_collect (curr_msg))
		      {
			curr_msg.set_id (consume_cnt + 1);
			collector.collect_msg (std::move (curr_msg));
			consume_cnt++;
		      }
		    else
		      lvlog (LogLv::DEBUG, " consume cnt:",
			     it_cnt_consumer.load (std::memory_order_relaxed),
			     " message not collected:", curr_msg.to_string ());
		  }
	      }

	    {
	      auto tmp = std::move (mq);
	    }
	  }
	else
	  {
	    lvlog (LogLv::DEBUG, "consumer_iter:",
		   it_cnt_consumer.load (std::memory_order_relaxed),
		   " mq not consumable,mq.size():", mq.size ());
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
	lvlog (LogLv::INFO, "switcher summary ",
	       "P:", it_cnt_producer.load (std::memory_order_relaxed),
	       " C:", it_cnt_consumer.load (std::memory_order_relaxed),
	       " S:", it_cnt_switcher.load (std::memory_order_relaxed),
	       " mqsize:", mq.size (), " nhandle:", collector.n_handlers (),
	       " nuser:", collector.n_users (),
	       " nchattitle:", collector.n_chat_titles ());
      }

      if (is_csm_mq.load (std::memory_order_acquire))
	{
	  lvlog (LogLv::INFO, "switcher cnt:",
		 it_cnt_switcher.load (std::memory_order_relaxed),
		 " has msg, consumer maybe handling...");
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
