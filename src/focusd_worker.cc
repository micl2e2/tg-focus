#include <mutex>
#include <atomic>
#include <thread>
#include <fmt/core.h>
#include <iostream>

#include "td_client.hh"
#include "focusd_state.hh"
#include "focus_filter.hh"

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

bool
is_tgfocus_msg (const TgMsg &msg)
{
  if (msg.get_chat_title ().find ("TG-FOCUS") != std::string::npos)
    return true;

  return false;
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

      if (!td_client.tried_create_collector)
	{
	  td_client.create_tgfocus_group ();
	  continue;
	}

      consume_existing_messages : {
	std::lock_guard<std::mutex> mq_guard (mq_lock);

	if (mq.size () > 0 && td_client.done_create_collector)
	  {
	    std::cerr << fmt::format (
	      "[CONSUMER {}] mq consumable, mq.size():{} ",
	      it_cnt_consumer.load (std::memory_order_relaxed), mq.size ())
		      << std::endl;

	    for (auto it = mq.begin (); it != mq.end (); it += 1)
	      {
		auto curr_msg = *it;
		if (!is_tgfocus_msg (curr_msg))
		  {
		    if (need_collect (curr_msg))
		      {
			td_client.collect_msg (std::move (curr_msg),
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
		else
		  {
		    // we dont deal or log tgfocus message.
		  }
	      }
	    mq.clear ();
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
  // size_t it_count = 0;
  while (true)
    {
      std::this_thread::sleep_for (std::chrono::seconds (5));

      {
	std::cout
	  << fmt::format ("[SWITCHER {}] P,{} C,{} S,{} check for switching...",
			  it_cnt_switcher.load (std::memory_order_relaxed),
			  it_cnt_producer.load (std::memory_order_relaxed),
			  it_cnt_consumer.load (std::memory_order_relaxed),
			  it_cnt_switcher.load (std::memory_order_relaxed))

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
      // it_count++;
    }
}
