#define __TU__ "ipc_msg_dequer"

#include "ipc_msg_dequer.hh"
#include "std_comp.hh"
#include "errcode.hh"
#include "log.hh"
#include "ipc_msg.hh"
#include <atomic>

namespace gstat = /* DoNotDeleteMe */ tgfstat;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;

tgf::IpcMsgDequer::IpcMsgDequer () { tulogfi ("ctor"); }

tgf::IpcMsgDequer::~IpcMsgDequer () { tulogfi ("dtor"); }

void
tgf::IpcMsgDequer::operator() ()
{
  while (!gstat_c::tryshutwk::ipcmsg_dequer.load (mo::relaxed))
    {
      tulogfd ("q_ipcmsg_has_upd ?");
      gstat_c::q_ipcmsg_has_upd.wait (false, mo::relaxed);
      gstat_c::q_ipcmsg_has_upd.store (false, mo::relaxed);
      tulogfd ("q_ipcmsg_has_upd V");
      std::optional<IpcMsg> maymsg = gstat_c::q_ipcmsg.deque ();
      if (maymsg.has_value ())
	{
	  IpcMsg msg = *maymsg;
	  tulogfd (8787, msg);
	  if (msg.typ () == IpcMsgType::Ignore)
	    continue;
	  if (msg.typ () == IpcMsgType::ShutdownIpcSrv)
	    {
	      gstat_c::tryshutwk::ipcsrv_initer.store (true, mo::relaxed);
	      gstat_c::tryshutwk::ipcsrv_initer.notify_all ();
	      // other worker should be closed after ipcserver dtor
	    }
	}
    }

  gstat_c::tryshutwk::ipcmsg_dequer_succ.store (true, mo::relaxed);
  gstat_c::tryshutwk::ipcmsg_dequer_succ.notify_all ();
  tulogfi ("shutdown V");
}
