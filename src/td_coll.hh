#ifndef hh_td_coll
#define hh_td_coll

#include "std_comp.hh"
#include "td_comp.hh"

#include <unordered_map>
#include <string>
#include <functional>

class TdCollector
{
  unique_ptr<td::ClientManager> __client;
  i32 __clientid;
  tapi::object_ptr<tapi::AuthorizationState> __auth_stat;
  u64 __curr_qry_id;
  u64 __auth_qry_id;
  unordered_map<u64, function<void (TdObjPtr)>> __cb;
  unordered_map<i64, tapi::object_ptr<tapi::user>> __users;
  unordered_map<i64, string> __chat_titles;
  // signal td client to send a request
  void send_query (tapi::object_ptr<tapi::Function> tdfn, function<void (TdObjPtr)> cbfn);
  // process response from td client, maybe for an automatic update, maybe for a
  // user's request.
  void process_response (td::ClientManager::Response response);
  // get user name from internal map
  string get_user_name (i64 user_id) const;
  // get chat title from internal map
  string get_chat_title (i64 chat_id) const;
  void process_update (TdObjPtr update);
  function<void (TdObjPtr)> auth_query_callback ();
  void on_authorization_state_update ();
  void check_authentication_error (TdObjPtr object);
  // td request id
  u64 next_query_id ();

public:
  bool tried_c_tgfchat{false};
  TdCollector () : __clientid (0), __curr_qry_id (0), __auth_qry_id (0) {}
  // initialize td client
  void init ();
  // create collector chat
  void try_create_tgfchat ();
  // send message to collector chat
  void collect_msg (const tgf::TgMsg &msg);
  void handle_tgfcmd (string &&incom_txt);
  // signal td client to fetch updates
  void fetch_updates ();
  bool is_auth{false};
  size_t n_handlers () { return this->__cb.size (); }
  size_t n_users () { return this->__users.size (); }
  size_t n_chat_titles () { return this->__chat_titles.size (); }
};

#endif
