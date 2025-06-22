#define __TU__ "coll_switcher"

#include "coll_switcher.hh"

#include "std_comp.hh"
#include "errcode.hh"
#include "log.hh"

#include <mutex>
#include <thread>

namespace gstat = /* DoNotDeleteMe */ tgfstat::c::d;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;

tgf::CollSwitcher::CollSwitcher () { tulogfi_cg (1, "ctor"); }

tgf::CollSwitcher::~CollSwitcher () { tulogfi_cg (1, "dtor"); }

void
tgf::CollSwitcher::operator() ()
{
  while (!gstat_c::tryshutwk::coll_switcher.load (mo::relaxed))
    {
      this_thread::sleep_for (chrono::seconds (5));

      {
	tgf::logfi_cg (1, "switcher summary", "P:", gstat::it_cnt_producer.load (mo::relaxed),
		       "C:", gstat::it_cnt_consumer.load (mo::relaxed), "S:", gstat::it_cnt_switcher.load (mo::relaxed),
		       "mqsize:", gstat::mq.size (), "nhandle:", gstat::collector.n_handlers (),
		       "nuser:", gstat::collector.n_users (), "nchattitle:", gstat::collector.n_chat_titles (),
		       "pause_do_csm_mq:", gstat::pause_do_csm_mq.load (mo::relaxed),
		       "do_csm_mq:", gstat::do_csm_mq.load (mo::relaxed));
      }

      if (!gstat::pause_do_csm_mq.load (mo::relaxed))
	{
	  if (gstat::do_csm_mq.load (mo::acquire))
	    {
	      tgf::logfi_cg (1, "switcher cnt:", gstat::it_cnt_switcher.load (mo::relaxed),
			     " has msg, consumer maybe handling...");
	      continue;
	    }

	  {
	    lock_guard<mutex> mq_guard (gstat::mq_lock);
	    if (gstat::mq.size () > 0)
	      {
		gstat::do_csm_mq.store (true, mo::release); // unlock
		gstat::do_csm_mq.notify_all ();
	      }
	  }
	}

      gstat::it_cnt_switcher.fetch_add (1, mo::relaxed);
    }

  tulogfd_cg (1, "closing producer...");
  gstat_c::tryshutwk::coll_producer.store (true, mo::relaxed);
  gstat_c::tryshutwk::coll_producer.notify_all ();
  gstat_c::tryshutwk::coll_producer_succ.wait (false, mo::relaxed);
  tulogfd_cg (1, "closing producer...done");
  tulogfd_cg (1, "closing consumer...");
  // foceably break consumer's workflow
  gstat::do_csm_mq.store (true, mo::relaxed);
  gstat::do_csm_mq.notify_all ();
  gstat_c::tryshutwk::coll_consumer.store (true, mo::relaxed);
  gstat_c::tryshutwk::coll_consumer.notify_all ();
  gstat_c::tryshutwk::coll_consumer_succ.wait (false, mo::relaxed);
  tulogfd_cg (1, "closing consumer...done");

  gstat_c::tryshutwk::coll_switcher_succ.store (true, mo::relaxed);
  gstat_c::tryshutwk::coll_switcher_succ.notify_all ();

  tulogfi_cg (1, "shutdown V");
}
