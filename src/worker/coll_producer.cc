#define __TU__ "coll_producer"

#include "coll_producer.hh"

#include "std_comp.hh"
#include "errcode.hh"
#include "log.hh"

namespace gstat = /* DoNotDeleteMe */ tgfstat::c::d;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;

tgf::CollProducer::CollProducer () { tulogfi_cg (1, "ctor"); }

tgf::CollProducer::~CollProducer () { tulogfi_cg (1, "dtor"); }

void
tgf::CollProducer::operator() ()
{
  while (!gstat_c::tryshutwk::coll_producer.load (mo::relaxed))
    {
      gstat::do_csm_mq.wait (true, mo::acquire);

      tulogfd_cg (1, "producer_iter:", gstat::it_cnt_producer.load (mo::relaxed), " mq size:", gstat::mq.size ());

      tulogfd_cg (1, "producer", "fetching...");
      gstat::collector.fetch_updates ();
      tulogfd_cg (1, "producer", "fetching...done");

      gstat::it_cnt_producer.fetch_add (1, mo::relaxed);
    }

  tulogfi_cg (1, "shutdowning...");
  gstat_c::tryshutwk::coll_producer_succ.store (true, mo::release);
  gstat_c::tryshutwk::coll_producer_succ.notify_all ();
  tulogfi_cg (1, "shutdown V");
}
