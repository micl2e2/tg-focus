#define __TU__ "ipc_server_initer"

#include "conn_listener.hh"

#include "std_comp.hh"
#include "errcode.hh"
#include "log.hh"
#include "common.hh"

#include "ipc_server_initer.hh"
#include "ipc_server.hh"
#include "ipc_msg.hh"

#include <atomic>
#include <memory>

namespace gstat = /* DoNotDeleteMe */ tgfstat;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;

tgf::IpcServerInitializer::IpcServerInitializer ()
{
  tulogfi ("ctor");
}

tgf::IpcServerInitializer::~IpcServerInitializer ()
{
  tulogfi ("dtor");
}

void
tgf::IpcServerInitializer::operator() ()
{
  std::shared_ptr<tgf::IpcServer> srv = std::make_shared<tgf::IpcServer> ();
  if (srv->ok ())
    {
      ConnListener worker
	= ConnListener (srv->ssock_lazy (), srv->ssock_immd ());
      std::jthread t (worker);
      ensure::is_valid_thread (t, tgf::EC::IPCSRCINITER_SYSCALL_THREAD);
      t.detach ();
    }
  else
    {
      // reaching here is a bug
      tulogfe (1122, srv->ec ());
      return;
    }
  gstat_c::tryshutwk::ipcsrv_initer.wait (false, mo::relaxed);

  gstat_c::tryshutwk::ipcsrv_initer_succ.store (true, mo::relaxed);
  gstat_c::tryshutwk::ipcsrv_initer_succ.notify_all ();
  tulogfi ("shutdown V");
}
