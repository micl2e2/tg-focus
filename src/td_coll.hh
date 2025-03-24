#ifndef hh_td_coll
#define hh_td_coll

#include <cstdlib>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>
#include <map>
#include <functional>
#include <atomic>
#include <chrono>
#include <iostream>

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include "std_comp.hh"
#include "td_comp.hh"

class TdCollector
{
public:
  bool tried_c_tgfchat{false};
  // bool done_create_collector{false};

  TdCollector () = default;

  //
  // initialize td client
  void init ();

  //
  // create collector chat
  void try_create_tgfchat ();
  //
  // send message to collector chat
  void collect_msg (const tgf::TgMsg &msg);

  void handle_tgfcmd (string &&incom_txt);

  //
  // signal td client to fetch updates
  void fetch_updates ();

  bool is_auth{false};

  size_t n_handlers () { return this->handlers_.size (); }

  size_t n_users () { return this->users_.size (); }

  size_t n_chat_titles () { return this->chat_title_.size (); }

private:
  unique_ptr<TdClient> client_manager_; // client manager obj
  int32_t client_id_{0};		// client id

  TdUniPtr<TdAuthStat> auth_state_;
  // td::tapi::int53 collector_id{0};
  uint64_t current_query_id_{0}; // increase by each query
  uint64_t auth_query_id_{0};

  map<uint64_t, function<void (TdObjPtr)>> handlers_;

  map<int64_t, TdUniPtr<TdUser>> users_;

  map<int64_t, string> chat_title_;

  //
  // signal td client to send a request
  void send_query (TdUniPtr<TdFunc> f, function<void (TdObjPtr)> handler);

  //
  // process response from td client, maybe for an automatic update, maybe for a
  // user's request.
  void process_response (TdClient::Response response);

  //
  // get user name from internal map
  string get_user_name (int64_t user_id) const;

  //
  // get chat title from internal map
  string get_chat_title (int64_t chat_id) const;

  void process_update (TdObjPtr update);

  auto auth_query_callback ();

  void on_authorization_state_update ();

  void check_authentication_error (TdObjPtr object);

  //
  // td request id
  uint64_t next_query_id ();
};

#endif
