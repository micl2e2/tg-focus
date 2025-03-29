#ifndef hh_td_auth
#define hh_td_auth

#include "stat.hh"
#include "td_comp.hh"

#include <unordered_map>
#include <string>
#include <functional>

namespace tgf {

class TdAuth
{
  unique_ptr<TdClient> __client;
  i32 __clientid;
  TdPtr<TdAuthStat> __auth_stat;
  bool __is_auth;
  u64 __curr_qry_id;
  unordered_map<u64, function<void (TdObjPtr)>> __cb;
  i32 __apiid;
  string __apihash;
  bool __use_our_api;
  void send_query (TdPtr<TdFunc> tdfn, function<void (TdObjPtr)> cbfn);
  void process_response (TdClient::Response response);
  void process_update (TdObjPtr update);
  function<void (TdObjPtr)> auth_query_callback ();
  void on_authorization_state_update ();
  u64 next_query_id () { return ++this->__curr_qry_id; }

public:
  TdAuth (bool useProvidedApiPass);
  ~TdAuth ();
  void loop ();
};
} // namespace tgf

#endif
