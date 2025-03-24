#define __TU__ "coll_initer"

#include "coll_initer.hh"

#include "std_comp.hh"
#include "errcode.hh"
#include "log.hh"
#include "locale_conf.hh"
#include "user_data.hh"
#include "coll_producer.hh"
#include "coll_consumer.hh"
#include "coll_switcher.hh"

#include <mutex>

namespace gstat = /* DoNotDeleteMe */ tgfstat::c::d;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;
namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

tgf::CollIniter::CollIniter ()
{
  tulogfi_cg (1,  "ctor");
}

tgf::CollIniter::~CollIniter ()
{
  tulogfi_cg (1,  "dtor");
}

void
tgf::CollIniter::operator() ()
{
  gstat_c::coll_initer_up.store (true, mo::relaxed);
  gstat_c::coll_initer_up.notify_all ();

  // typically initialized by tgf-conf
  tgf::PREFER_LANG = gstat_all::userdata.get_pref_lang ();

  if (!tgf::try_ensure_locale ())
    tgf::logfi_cg (1, "Available utf8 locales not found");
  else
    tgf::logfi_cg (1, tgf::HOST_LANG);

  while (!gstat_all::userdata.get_auth_hint ())
    {
      tgf::logfi_cg (1, "Waiting for authorization");
      this_thread::sleep_for (chrono::seconds (3));
    }

  if (!gstat_all::userdata.get_auth_hint ())
    {
      tgf::logfi_cg (1, "Not authorized");
      return;
    }

  gstat::collector.init ();

  CollProducer p = CollProducer ();
  CollConsumer c = CollConsumer ();
  CollSwitcher s = CollSwitcher ();

  jthread producer (p);
  jthread switcher (c);
  jthread consumer (s);

  gstat_c::tryshutwk::coll_initer.wait (false, mo::relaxed);

  // shall be switcher who handles producer/consumer's closing
  tulogfd_cg (1, "closing switcher...");
  gstat_c::tryshutwk::coll_switcher.store (true, mo::relaxed);
  gstat_c::tryshutwk::coll_switcher.notify_all ();
  gstat_c::tryshutwk::coll_switcher_succ.wait (false, mo::relaxed);
  tulogfd_cg (1, "closing switcher...done");

  gstat_c::tryshutwk::coll_initer_succ.store (true, mo::relaxed);
  gstat_c::tryshutwk::coll_initer_succ.notify_all ();

  gstat_c::coll_initer_up.store (false, mo::relaxed);
  gstat_c::coll_initer_up.notify_all ();
  tulogfi_cg (1, "shutdown V");
}
