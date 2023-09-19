#include <atomic>
#include <chrono>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <format>
#include <string_view>
#include <thread>
#include <unistd.h>
#include <vector>

#include "common.hh"
#include "toml.hpp"

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include "focus_filter.hh"
#include "tg_focus_data.hh"

namespace td_api = td::td_api;

std::atomic<bool> is_login{false};
std::atomic<bool> is_tdlib_closed{false};

TgFocusData tf_data{std::nullopt, false};

template <class... Args>
void
log (std::format_string<Args...> fmt, Args &&...args)
{
  constexpr std::string_view header{"[tf-conf] "};
  std::cout << header << std::format (fmt, args...) << std::endl;
}

template <class... Args>
void
log_flush (std::format_string<Args...> fmt, Args &&...args)
{
  constexpr std::string_view header{"[tf-conf] "};
  std::cout << header << std::format (fmt, args...) << std::flush;
}

class TdAuth
{
public:
  TdAuth ()
  {
    td::ClientManager::execute (
      td_api::make_object<td_api::setLogVerbosityLevel> (1));
    client_manager_ = std::make_unique<td::ClientManager> ();
    client_id_ = client_manager_->create_client_id ();
    send_query (td_api::make_object<td_api::getOption> ("version"), {});
  }

  ~TdAuth ()
  {
    send_query (td_api::make_object<td_api::close> (), [] (Object obj) {
      if (obj->get_id () == td_api::ok::ID)
	log ("Closing...");
      // std::cerr << "close cb, obj id:" << obj->get_id () << std::endl;
      // is_tdlib_closed.store (true, std::memory_order_release);
    });

    while (!is_tdlib_closed.load (std::memory_order_acquire))
      {
	// std::cerr << "try closing...\n";
	auto response = client_manager_->receive (60);
	if (response.object)
	  {
	    process_response (std::move (response));
	  }
      }
  }

  void loop ()
  {
    while (!is_login.load (std::memory_order_acquire))
      {
	auto response = client_manager_->receive (60);
	if (response.object)
	  {
	    process_response (std::move (response));
	  }
      }
    log ("Logined successfully!");
    // std::cout << "[td-auth] logined successfully!\n";
  }

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
		   std::function<void (Object)> handler)
  {
    auto query_id = next_query_id ();
    if (handler)
      {
	handlers_.emplace (query_id, std::move (handler));
      }
    client_manager_->send (client_id_, query_id, std::move (f));
  }

  void process_response (td::ClientManager::Response response)
  {
    if (!response.object)
      {
	return;
      }
    if (response.request_id == 0)
      {
	return process_update (std::move (response.object));
      }
    auto it = handlers_.find (response.request_id);
    if (it != handlers_.end ())
      {
	it->second (std::move (response.object));
	handlers_.erase (it);
      }
  }

  std::string get_user_name (std::int64_t user_id) const
  {
    auto it = users_.find (user_id);
    if (it == users_.end ())
      {
	return "unknown user";
      }
    return it->second->first_name_ + " " + it->second->last_name_;
  }

  std::string get_chat_title (std::int64_t chat_id) const
  {
    auto it = chat_title_.find (chat_id);
    if (it == chat_title_.end ())
      {
	return "unknown chat";
      }
    return it->second;
  }

  void process_update (td_api::object_ptr<td_api::Object> update)
  {
    using td::td_api::updateAuthorizationState;
    using td::td_api::updateChatTitle;
    using td::td_api::updateNewChat;
    using td::td_api::updateNewMessage;
    using td::td_api::updateUser;

    switch (update->get_id ())
      {
	case updateAuthorizationState::ID: {
	  auto casted = static_cast<updateAuthorizationState *> (update.get ());
	  this->auth_state_ = std::move (casted->authorization_state_);
	  this->on_authorization_state_update ();
	  break;
	}
      }
  }

  auto auth_query_callback ()
  {
    return [this] (Object object) {
      if (object->get_id () == td_api::ok::ID)
	{
	  // std::cout << "OK!" << std::endl;
	}
      else if (object->get_id () == td_api::error::ID)
	{
	  auto error = td::move_tl_object_as<td_api::error> (object);
	  log ("ERROR: {}", error->message_);
	  // std::cout << "auth err: " << to_string (error) << std::flush;
	  on_authorization_state_update ();
	}
      else
	{
	  std::cerr << "unexpected auth callback id: " << object->get_id ()
		    << std::endl;
	}
    };
  }

  void on_authorization_state_update ()
  {
    switch (this->auth_state_->get_id ())
      {
	case td_api::authorizationStateReady::ID: {
	  // this means: api id/hash correct, phone and vcode correct

	  this->is_authorized = true;

	  // persistant
	  tf_data.set_api_id (std::format ("{}", this->api_id_));
	  tf_data.set_api_hash (std::move (this->api_hash_));
	  tf_data.set_auth_hint (true);

	  is_login.store (true, std::memory_order_release);

	  break;
	}

	// FIXME: auth-reset should make use of this event
	// case td_api::authorizationStateLoggingOut::ID: {
	//   this->is_authorized = false;
	//   std::cout << "Logging out" << std::endl;
	//   break;
	// }

	case td_api::authorizationStateClosing::ID: {
	  // std::cout << "Closing" << std::endl;
	  break;
	}

	case td_api::authorizationStateClosed::ID: {
	  log ("Closed");
	  this->is_authorized = false;
	  is_tdlib_closed.store (true, std::memory_order_release);
	  // std::cout << "Actual Closed!" << std::endl;
	  break;
	}

	case td_api::authorizationStateWaitPhoneNumber::ID: {
	  log_flush ("Enter phone number: ");
	  // std::cout << "Enter phone number: " << std::flush;
	  std::string phone_number;
	  std::cin >> phone_number;
	  send_query (
	    td_api::make_object<td_api::setAuthenticationPhoneNumber> (
	      phone_number, nullptr),
	    auth_query_callback ());
	  break;
	}

	case td_api::authorizationStateWaitEmailAddress::ID: {
	  log_flush ("Enter email address: ");
	  // std::cout << "Enter email address: " << std::flush;
	  std::string email_address;
	  std::cin >> email_address;
	  send_query (
	    td_api::make_object<td_api::setAuthenticationEmailAddress> (
	      email_address),
	    auth_query_callback ());
	  break;
	}

	case td_api::authorizationStateWaitEmailCode::ID: {
	  log_flush ("Enter email authentication code: ");
	  // std::cout << "Enter email authentication code: " << std::flush;
	  std::string code;
	  std::cin >> code;
	  send_query (
	    td_api::make_object<td_api::checkAuthenticationEmailCode> (
	      td_api::make_object<td_api::emailAddressAuthenticationCode> (
		code)),
	    auth_query_callback ());
	  break;
	}

	case td_api::authorizationStateWaitCode::ID: {
	  log_flush ("Enter authentication code: ");
	  // std::cout << "Enter authentication code: " << std::flush;
	  std::string code;
	  std::cin >> code;
	  send_query (td_api::make_object<td_api::checkAuthenticationCode> (
			code),
		      auth_query_callback ());
	  break;
	}

	case td_api::authorizationStateWaitPassword::ID: {
	  log_flush ("Enter authentication password: ");
	  // std::cout << "Enter authentication password: " << std::flush;
	  std::string password;
	  std::getline (std::cin, password);
	  send_query (td_api::make_object<td_api::checkAuthenticationPassword> (
			password),
		      auth_query_callback ());
	  break;
	}

	case td_api::authorizationStateWaitOtherDeviceConfirmation::ID: {
	  auto casted = static_cast<
	    td_api::authorizationStateWaitOtherDeviceConfirmation *> (
	    this->auth_state_.get ());
	  std::cout << "Confirm this login link on another device: "
		    << casted->link_ << std::endl;
	  break;
	}

	// always first event
	case td_api::authorizationStateWaitTdlibParameters::ID: {
	  std::int32_t api_id;
	  std::string may_api_id;
	  std::string api_hash;

	  if (tf_data.get_auth_hint ())
	    {
	      api_id = tf_data.get_api_id_as_int32 ();
	      api_hash = tf_data.get_api_hash ();
	    }
	  else
	    {
	      log_flush ("Enter api id: ");
	      // std::cout << "Enter api id: " << std::flush;
	      std::cin >> may_api_id;

	      if (is_valid_int32 (may_api_id))
		api_id = sstr_to_int32 (may_api_id);
	      else
		api_id = 0;

	      log_flush ("Enter api hash: ");
	      // std::cout << "Enter api hash: " << std::flush;
	      std::cin >> api_hash;
	    }

	  auto request = td_api::make_object<td_api::setTdlibParameters> ();
	  request->database_directory_ = tf_data.path_tddata ();
	  request->use_message_database_ = true;
	  request->use_secret_chats_ = true;
	  request->api_id_ = api_id;
	  request->api_hash_ = api_hash;
	  request->system_language_code_ = "en";
	  request->device_model_ = "TG-FOCUS";
	  request->application_version_ = "1.1.817-beta1";
	  request->enable_storage_optimizer_ = true;

	  this->api_id_ = api_id;
	  this->api_hash_ = api_hash;

	  send_query (std::move (request), auth_query_callback ());
	  break;
	}
      }
  }

  std::uint64_t next_query_id () { return ++this->curr_query_id; }
};

int
print_usage (char *argv[])
{
  printf ("Usage: %s auth/filters\n", argv[0]);
  return 0;
}

int
handle_auth ()
{
  TdAuth td_auth;
  td_auth.loop ();
  return 0;
}

int
handle_auth_reset ()
{
  std::error_code ec;

  // first
  tf_data.set_auth_hint (false);

  // second
  auto del_res = std::filesystem::remove_all (tf_data.path_tddata (), ec);
  if (del_res == static_cast<std::uintmax_t> (-1))
    return 1;

  log ("reset successfully");

  return 0;
}

int
handle_filters ()
{
  if (!tf_data.prepare_filters_tmp ())
    return 2;

  auto fpath = tf_data.path_filters_tmp ();
  auto fpath_cstr = fpath.c_str ();
  std::system (std::format ("$EDITOR {}", fpath_cstr).c_str ());

  log ("Verifying filters...");

  bool is_invalid_toml = false;
  toml::value res;

  try
    {
      res = toml::parse (fpath_cstr);
    }
  catch (const std::exception &ex)
    {
      is_invalid_toml = true;
    }

  if (is_invalid_toml)
    log ("ERROR: Invalid toml");
  else
    {
      FileReader reader{fpath_cstr};
      if (FocusFilterList::is_valid (reader.read_to_string ().value_or ("-")))
	{
	  log ("Saving filters...");
	  tf_data.set_filters (tf_data.get_filters_tmp ());
	}
      else
	log ("ERROR: Invalid filters");
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  if (argc != 2)
    return print_usage (argv);

  std::string subcmd = argv[1];

  if (subcmd == "auth-reset")
    return handle_auth_reset ();

  if (subcmd == "auth")
    return handle_auth ();

  if (subcmd == "filters")
    return handle_filters ();

  return print_usage (argv);
}
