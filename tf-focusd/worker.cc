#include <mutex>
#include <atomic>
#include <thread>
#include <fmt/core.h>
#include <iostream>

#include "state.hh"
#include "focus_filter.hh"
#include "tf_msg.hh"

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

      collector.fetch_updates ();

      it_cnt_producer.fetch_add (1, std::memory_order_relaxed);
    }
}

bool
need_collect (const TgMsg &msg)
{
  // FileReader freader{"filters.toml"};

  // if (auto tomlstr = freader.read_to_string ())
  {
    auto tomlstr = tf_data.get_filters ();
    // std::cerr << "filters reloaded: " << *tomlstr << std::endl;
    std::cerr << fmt::format ("[CONSUMER {}] filters reloaded...",
			      it_cnt_consumer.load (std::memory_order_relaxed))
	      << std::endl;
    auto fcf_list = FocusFilterList (tomlstr);
    if (fcf_list.is_tgmsg_match (msg))
      return true;
  }

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
	    std::cerr << fmt::format (
	      "[CONSUMER {}] mq consumable, mq.size():{} ",
	      it_cnt_consumer.load (std::memory_order_relaxed), mq.size ())
		      << std::endl;

	    for (auto it = mq.begin (); it != mq.end (); it += 1)
	      {
		auto curr_msg = *it;
		if (!curr_msg.is_from_tgfocus ())
		  {
		    if (need_collect (curr_msg))
		      {
			collector.collect_msg (std::move (curr_msg),
					       consume_cnt);
			consume_cnt++;
		      }
		    else
		      std::cerr
			<< fmt::format ("[CONSUMER,{}] message not collected: ",
					it_cnt_consumer.load (
					  std::memory_order_relaxed))
			<< curr_msg << std::endl;
		  }
	      }

	    {
	      auto tmp = std::move (mq);
	    }
	  }
	else
	  {
	    std::cerr << fmt::format (
	      "[CONSUMER {}] mq not consumable, mq.size():{} ",
	      it_cnt_consumer.load (std::memory_order_relaxed), mq.size ())
		      << std::endl;
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
	std::cout << fmt::format (
	  "[SWITCHER {}] P,{} C,{} S,{} nhandle:{},nuser:{},nchattitle:{}  "
	  "check for switch...",
	  it_cnt_switcher.load (std::memory_order_relaxed),
	  it_cnt_producer.load (std::memory_order_relaxed),
	  it_cnt_consumer.load (std::memory_order_relaxed),
	  it_cnt_switcher.load (std::memory_order_relaxed),
	  collector.n_handlers (), collector.n_users (),
	  collector.n_chat_titles ())
		  << std::endl;
      }

      if (is_csm_mq.load (std::memory_order_acquire))
	{
	  std::cout
	    << fmt::format ("[SWITCHER {}] has msg, consumer maybe handling...",
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
    }
}
