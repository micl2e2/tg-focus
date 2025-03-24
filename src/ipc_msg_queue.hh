#ifndef hh_ipcmsgqueue
#define hh_ipcmsgqueue

#include <vector>
#include <mutex>
#include <optional>
#include "ipc_msg.hh"

namespace tgf {

class IpcMsgQueue
{
  std::vector<IpcMsg> queue_;
  std::mutex lck_;

public:
  IpcMsgQueue () { queue_ = std::vector<IpcMsg>{}; }
  bool enqueue (IpcMsg &&cmd);
  std::optional<tgf::IpcMsg> deque ();
};

} // namespace tgf

#endif
