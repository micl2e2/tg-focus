#ifndef hh_coll_consumer
#define hh_coll_consumer

#include "stat.hh"
#include "tg_msg.hh"

namespace tgf {

namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

/*
 */
class CollConsumer
{
public:
  CollConsumer ();
  ~CollConsumer ();
  void operator() ();
};

bool
need_collect (const TgMsg &msg);

} // namespace tgf

#endif
