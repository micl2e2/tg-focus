#ifndef hh_ipcmsgqueue
#define hh_ipcmsgqueue

#include <vector>
#include <mutex>
#include <optional>
#include "ipc_msg.hh"

namespace tgf {

class IpcMsgQueue
{
  std::vector<IpcMsg> __queue;
  std::mutex __lck;

public:
  IpcMsgQueue () { __queue = std::vector<IpcMsg>{}; }
  bool enqueue (IpcMsg &&cmd);
  std::optional<tgf::IpcMsg> deque ();
};

} // namespace tgf

#endif
