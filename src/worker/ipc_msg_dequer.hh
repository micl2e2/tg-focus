#ifndef hh_ipc_msg_dequer
#define hh_ipc_msg_dequer

#include "../stat.hh"

namespace tgf {

namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

/*
  A worker whose responsibility is to handle ipc msg queue.
 */
class IpcMsgDequer
{
public:
  IpcMsgDequer ();
  ~IpcMsgDequer ();
  void operator() ();
};

} // namespace tgf

#endif
