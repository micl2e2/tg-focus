#define __TU__ "ipc_msg_queue"

#include "ipc_msg_queue.hh"
#include "log.hh"
#include "stat.hh"
#include <atomic>

bool
tgf::IpcMsgQueue::enqueue (tgf::IpcMsg &&cmd)
{
  std::lock_guard<std::mutex> g (__lck);
  tulogfd ("IpcMsgQueue::enqueue", "queue size", __queue.size ());

  __queue.emplace_back (cmd);
  tgfstat::c::q_ipcmsg_has_upd.store (true, std::memory_order_relaxed);
  tgfstat::c::q_ipcmsg_has_upd.notify_all ();

  return true;
}

std::optional<tgf::IpcMsg>
tgf::IpcMsgQueue::deque ()
{
  std::lock_guard<std::mutex> g (__lck);
  tulogfd ("IpcMsgQueue::deque", "queue size", __queue.size ());

  if (__queue.size () > 0)
    {
      tgf::IpcMsg cmd = __queue.back ();
      __queue.pop_back ();
      return cmd;
    }
  return std::nullopt;
}
