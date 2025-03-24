#ifndef hh_ipc_conn_handler
#define hh_ipc_conn_handler

#include "../ipc_msg_queue.hh"
#include "../stat.hh"

namespace tgf {

namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

/*
  A worker whose responsibility is to handle incoming ipc msg.
 */
class IpcConnHandler
{
  int connfd_;
  bool conn_handled;
  bool immd_;

public:
  IpcConnHandler ();
  ~IpcConnHandler ();
  IpcConnHandler (int fd, bool immd = false);
  void operator() ();
};

} // namespace tgf

#endif
