#define __TU__ "coll_consumer"

#include "coll_consumer.hh"

#include "std_comp.hh"
#include "errcode.hh"
#include "log.hh"
#include "filter.hh"

#include <mutex>

namespace gstat = /* DoNotDeleteMe */ tgfstat::c::d;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;

tgf::CollConsumer::CollConsumer () { tulogfi_cg (1, "ctor"); }

tgf::CollConsumer::~CollConsumer () { tulogfi_cg (1, "dtor"); }

bool
tgf::need_collect (const tgf::TgMsg &msg)
{
  auto tomlstr = gstat_all::userdata.get_filters ();
  tgf::logfi_cg (1, "consumer_iter:", gstat::it_cnt_consumer.load (mo::relaxed),
		 " filters reloaded");

  auto filterg = tgf::FilterGroupToml (tomlstr);
  if (filterg.mtch_tgmsg (msg))
    return true;

  return false;
}

void
tgf::CollConsumer::operator() ()
{
  int consume_cnt = 0;

  while (!gstat_c::tryshutwk::coll_consumer.load (mo::relaxed))
    {
      tulogfi_cg (1, 9898);
      // std::this_thread::sleep_for (std::chrono::seconds (1));
      gstat::do_csm_mq.wait (false, mo::acquire);

      bool isValid = gstat_all::userdata.is_tgfid_valid ();
      bool isTriedCreate = gstat::collector.tried_c_tgfchat;
      if (!isValid && !isTriedCreate)
	{
	  gstat::collector.try_create_tgfchat ();
	  continue;
	}

      // try consume mq
      {
	lock_guard<mutex> mq_guard (gstat::mq_lock);

	if (gstat::mq.size () > 0 && gstat_all::userdata.is_tgfid_valid ())
	  {
	    tulogfd_cg (1, "consumer_iter:",
			gstat::it_cnt_consumer.load (mo::relaxed),
			" mq consumable, mq.size():", gstat::mq.size ());

	    // !!!BLOCKING
	    for (auto it = gstat::mq.begin (); it != gstat::mq.end (); it += 1)
	      {
		// if so, give up consuming
		if (gstat_c::tryshutwk::coll_consumer.load (mo::relaxed))
		  break;
		auto curr_msg = *it;
		// break; // stop collect
		if (!curr_msg.is_from_tgfocus ())
		  {
		    // FIXME: too many disk io incurred by need_collect
		    if (need_collect (curr_msg))
		      {
			this_thread::sleep_for (chro::seconds (2));
			curr_msg.set_id (consume_cnt + 1);
			gstat::collector.collect_msg (move (curr_msg));
			consume_cnt++;
		      }
		    else
		      tulogfd_cg (1, " consume cnt:",
				  gstat::it_cnt_consumer.load (mo::relaxed),
				  " message not collected:",
				  curr_msg.to_string ());
		  }
	      }

	    {
	      auto tmp = move (gstat::mq);
	    }
	  }
	else
	  {
	    tulogfd_cg (1, "consumer_iter:",
			gstat::it_cnt_consumer.load (mo::relaxed),
			" mq not consumable,mq.size():", gstat::mq.size ());
	  }
      }

      gstat::it_cnt_consumer.fetch_add (1, mo::relaxed);
      gstat::do_csm_mq.store (false, mo::release);
      gstat::do_csm_mq.notify_all ();
    }

  gstat_c::tryshutwk::coll_consumer_succ.store (true, mo::relaxed);
  gstat_c::tryshutwk::coll_consumer_succ.notify_all ();
  tulogfi_cg (1, "shutdown V");
}
