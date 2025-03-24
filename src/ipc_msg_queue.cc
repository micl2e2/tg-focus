#define __TU__ "ipc_msg_queue"

#include "ipc_msg_queue.hh"
#include "log.hh"
#include "stat.hh"
#include <atomic>

bool
tgf::IpcMsgQueue::enqueue (tgf::IpcMsg &&cmd)
{
  std::lock_guard<std::mutex> g (lck_);
  tulogfd ( "IpcMsgQueue::enqueue", "queue size", queue_.size ());

  queue_.emplace_back (cmd);
  tgfstat::c::q_ipcmsg_has_upd.store (true, std::memory_order_relaxed);
  tgfstat::c::q_ipcmsg_has_upd.notify_all ();

  return true;
}

std::optional<tgf::IpcMsg>
tgf::IpcMsgQueue::deque ()
{
  std::lock_guard<std::mutex> g (lck_);
  tulogfd ( "IpcMsgQueue::deque", "queue size", queue_.size ());

  if (queue_.size () > 0)
    {
      tgf::IpcMsg cmd = queue_.back ();
      queue_.pop_back ();
      return cmd;
    }
  return std::nullopt;
}
