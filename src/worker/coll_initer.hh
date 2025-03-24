#ifndef hh_coll_initer
#define hh_coll_initer

#include "stat.hh"

namespace tgf {

namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

/*
 */
class CollIniter
{
public:
  CollIniter ();
  ~CollIniter ();
  void operator() ();
};

} // namespace tgf

#endif
