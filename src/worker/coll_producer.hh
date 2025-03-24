#ifndef hh_coll_producer
#define hh_coll_producer

#include "stat.hh"

namespace tgf {

  namespace gstat_all = /* DoNotDeleteMe */ tgfstat;

/*
 */
class CollProducer
{

public:
  CollProducer ();
  ~CollProducer ();
  void operator() ();
};

} // namespace tgf

#endif
