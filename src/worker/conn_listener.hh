#ifndef hh_conn_listener
#define hh_conn_listener

#include "stat.hh"

namespace tgf {

namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

/*
  A worker whose responsibility is to handle incoming ipc msg.
 */
class ConnListener
{
  int sock_lazy_;
  int sock_immd_;

public:
  ConnListener ();
  ~ConnListener ();
  ConnListener (int lazy, int immd) : sock_lazy_ (lazy), sock_immd_ (immd){};
  void operator() ();
};

} // namespace tgf

#endif
