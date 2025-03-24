#ifndef hh_ipc_server_initer
#define hh_ipc_server_initer

#include "stat.hh"

namespace tgf {

namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

/*
  A worker whose responsibility is to bring a daemon up.
 */
class IpcServerInitializer
{
public:
  IpcServerInitializer ();
  ~IpcServerInitializer ();
  void operator() ();
};

} // namespace tgf

#endif
