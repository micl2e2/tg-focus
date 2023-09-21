#ifndef _TD_AUTH_H
#define _TD_AUTH_H

#include "tfconf_state.hh"

//

#include <cstdlib>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>
#include <map>
#include <functional>
#include <atomic>
#include <fmt/core.h>
#include <chrono>
#include <iostream>

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

namespace td_api = td::td_api;

class TdAuth
{
public:
  TdAuth ();

  ~TdAuth ();

  void loop ();

private:
  using Object = td_api::object_ptr<td_api::Object>;
  std::unique_ptr<td::ClientManager> client_manager_; // client manager obj
  std::int32_t client_id_{0};			      // client id

  td_api::object_ptr<td_api::AuthorizationState> auth_state_;
  bool is_authorized{false};
  std::uint64_t curr_query_id{0}; // increase by each query

  std::map<std::uint64_t, std::function<void (Object)>> handlers_;

  std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;

  std::map<std::int64_t, std::string> chat_title_;

  std::int32_t api_id_;
  std::string api_hash_;

  void send_query (td_api::object_ptr<td_api::Function> f,
		   std::function<void (Object)> handler);

  void process_response (td::ClientManager::Response response);

  std::string get_user_name (std::int64_t user_id) const;

  std::string get_chat_title (std::int64_t chat_id) const;

  void process_update (td_api::object_ptr<td_api::Object> update);

  auto auth_query_callback ();

  void on_authorization_state_update ();

  std::uint64_t next_query_id () { return ++this->curr_query_id; }
};

#endif
