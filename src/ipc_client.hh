#include "errcode.hh"
#include "common.hh"
#include "ipc_msg.hh"
#include <optional>

namespace tgf {

class IpcClient
{
  int __csock;
  ErrCode __ec;

public:
  /*
   * After constructed, there should be a alive connection to the server, i.e.
   * valid csock
   */
  IpcClient ();
  ~IpcClient ();
  inline bool active () const { return __csock >= 0 && __ec == ErrCode::NOERR; }
  inline ErrCode ec () const { return __ec; }
  std::optional<IpcMsg> send_ipcmsg_sync (IpcMsg &&msg) noexcept;
};

} // namespace tgf
