#ifndef _TD_COLLECTOR_H
#define _TD_COLLECTOR_H

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

#include "tgf_msg.hh"
#include "tgf_data.hh"

namespace td_api = td::td_api;

class TdCollector
{
public:
  bool tried_create_tgfchat{false};
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
  void collect_msg (const TgMsg &msg, size_t c_count);

  //
  // signal td client to fetch updates
  void fetch_updates ();

  bool is_authorized{false};

  size_t n_handlers () { return this->handlers_.size (); }

  size_t n_users () { return this->users_.size (); }

  size_t n_chat_titles () { return this->chat_title_.size (); }

private:
  using Object = td_api::object_ptr<td_api::Object>;
  std::unique_ptr<td::ClientManager> client_manager_; // client manager obj
  std::int32_t client_id_{0};			      // client id

  td_api::object_ptr<td_api::AuthorizationState> auth_state_;
  // td::td_api::int53 collector_id{0};
  std::uint64_t current_query_id_{0}; // increase by each query
  std::uint64_t authentication_query_id_{0};

  std::map<std::uint64_t, std::function<void (Object)>> handlers_;

  std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;

  std::map<std::int64_t, std::string> chat_title_;

  //
  // signal td client to send a request
  void send_query (td_api::object_ptr<td_api::Function> f,
		   std::function<void (Object)> handler);

  //
  // process response from td client, maybe for an automatic update, maybe for a
  // user's request.
  void process_response (td::ClientManager::Response response);

  //
  // get user name from internal map
  std::string get_user_name (std::int64_t user_id) const;

  //
  // get chat title from internal map
  std::string get_chat_title (std::int64_t chat_id) const;

  void process_update (td_api::object_ptr<td_api::Object> update);

  auto auth_query_callback ();

  void on_authorization_state_update ();

  void check_authentication_error (Object object);

  //
  // td request id
  std::uint64_t next_query_id ();
};

#endif
