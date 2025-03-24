#ifndef hh_coll_switcher
#define hh_coll_switcher

#include "stat.hh"

namespace tgf {

namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

class CollSwitcher
{
public:
  CollSwitcher ();
  ~CollSwitcher ();
  void operator() ();
};

} // namespace tgf

#endif
