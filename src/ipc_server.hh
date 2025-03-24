#ifndef hh_ipc_server
#define hh_ipc_server

#include "errcode.hh"
#include "worker/ipc_server_initer.hh"

namespace tgf {

class IpcServer
{
  int __domain;
  // daemon server socket
  int __ssock_lazy;
  // dedicated for immediately process ipc msg
  int __ssock_immd;
  ErrCode __ec;

public:
  IpcServer ();
  ~IpcServer ();
  inline ErrCode ec () const { return __ec; }
  inline int ssock_lazy () const { return __ssock_lazy; }
  inline int ssock_immd () const { return __ssock_immd; }
  bool ok () { return __ssock_lazy >= 0 && __ssock_immd >= 0; };
};

} // namespace tgf

#endif
