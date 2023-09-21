#ifndef _TD_AUTH_H
#define _TD_AUTH_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include "tf_data.hh"

#include "tgfocus_state.hh"
extern std::vector<TgMsg> mq;
extern std::mutex mq_lock;
extern std::atomic<bool> is_csm_mq;
extern std::atomic<bool> is_tdlib_auth;
extern TgFocusData tf_data;
extern std::atomic<int> producer_hang_period;

namespace td_api = td::td_api;

class TdAuth
{
public:
  TdAuth ()
  {
    td::ClientManager::execute (
      td_api::make_object<td_api::setLogVerbosityLevel> (2));
    client_manager_ = std::make_unique<td::ClientManager> ();
    client_id_ = client_manager_->create_client_id ();
    send_query (td_api::make_object<td_api::getOption> ("version"), {});
  }

  void fetch_updates ()
  {
    // while (true)
    {
      auto response = client_manager_->receive (60);
      if (response.object)
	{
	  process_response (std::move (response));
	}
      else
	{
	  std::cout << "recv timeout\n";
	  // break;
	}
    }
  }

private:
  using Object = td_api::object_ptr<td_api::Object>;
  std::unique_ptr<td::ClientManager> client_manager_; // client manager obj
  std::int32_t client_id_{0};			      // client id

  td_api::object_ptr<td_api::AuthorizationState> auth_state_;
  bool are_authorized_{false};
  bool need_restart_{false};
  std::uint64_t current_query_id_{0}; // increase by each query
  std::uint64_t authentication_query_id_{0};

  std::map<std::uint64_t, std::function<void (Object)>> handlers_;

  std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;

  std::map<std::int64_t, std::string> chat_title_;

  void restart ()
  {
    client_manager_.reset ();
    *this = TdAuth ();
  }

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
    // std::cout << response.request_id << " " << to_string(response.object) <<
    // std::endl;
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

  void check_updates ()
  {
    while (true)
      {
	auto response = client_manager_->receive (30);
	if (response.object)
	  {
	    std::cout << "has resp\n";
	    process_response (std::move (response));
	  }
	else
	  {
	    std::cout << "recv timeout\n";
	    // break;
	  }
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

	case updateNewChat::ID: {
	  auto casted = static_cast<updateNewChat *> (update.get ());
	  this->chat_title_[casted->chat_->id_] = casted->chat_->title_;
	  break;
	}

	case updateChatTitle::ID: {
	  std::cout << "yes!\n";
	  auto casted = static_cast<updateChatTitle *> (update.get ());
	  this->chat_title_[casted->chat_id_] = casted->title_;
	  break;
	}

	case updateUser::ID: {
	  std::cout << "yes2!\n";
	  auto casted = static_cast<updateUser *> (update.get ());
	  auto user_id = casted->user_->id_;
	  std::cout << "yes2!\n" << user_id;
	  this->users_[user_id] = std::move (casted->user_);
	  break;
	}

	//           [this](td_api::updateChatTitle &update_chat_title) {
	//             chat_title_[update_chat_title.chat_id_] =
	//                 update_chat_title.title_;
	//           },
	//           [this](td_api::updateUser &update_user) {
	//             auto user_id = update_user.user_->id_;
	//             users_[user_id] = std::move(update_user.user_);
	//           },

	case updateNewMessage::ID: {
	  using td::td_api::messageSenderChat;
	  using td::td_api::messageSenderUser;
	  auto casted = static_cast<updateNewMessage *> (update.get ());
	  auto chat_id = casted->message_->chat_id_;
	  std::string sender_name;
	  auto sender_id = std::move (casted->message_->sender_id_);
	  switch (sender_id->get_id ())
	    {
	      case messageSenderUser::ID: {
		auto casted
		  = static_cast<messageSenderUser *> (sender_id.get ());
		sender_name = get_user_name (casted->user_id_);
		break;
	      }
	      case messageSenderChat::ID: {
		auto casted
		  = static_cast<messageSenderChat *> (sender_id.get ());
		sender_name = get_chat_title (casted->chat_id_);
		break;
	      }
	    }
	  std::string text;
	  if (casted->message_->content_->get_id () == td_api::messageText::ID)
	    {
	      text = static_cast<td_api::messageText &> (
		       *casted->message_->content_)
		       .text_->text_;
	    }
	  std::cout << "Receive message: "
		       "[chat_id:"
		    << chat_id << ", title:" << chat_title_[chat_id]
		    << "] "
		       "[from:"
		    << sender_name << "] [" << text << "]" << std::endl;
	  break;
	}
      }
  }

  auto create_authentication_query_handler ()
  {
    return [this, id = authentication_query_id_] (Object object) {
      if (id == authentication_query_id_)
	{
	  check_authentication_error (std::move (object));
	}
    };
  }

  void on_authorization_state_update ()
  {
    this->authentication_query_id_++;

    switch (this->auth_state_->get_id ())
      {
	case td_api::authorizationStateReady::ID: {
	  this->are_authorized_ = true;
	  std::cout << "Authorization is completed" << std::endl;
	  break;
	}
	case td_api::authorizationStateLoggingOut::ID: {
	  this->are_authorized_ = false;
	  std::cout << "Logging out" << std::endl;
	  break;
	}
	case td_api::authorizationStateClosing::ID: {
	  std::cout << "Closing" << std::endl;
	  break;
	}
	case td_api::authorizationStateClosed::ID: {
	  are_authorized_ = false;
	  need_restart_ = true;
	  std::cout << "Terminated" << std::endl;
	  break;
	}
	case td_api::authorizationStateWaitPhoneNumber::ID: {
	  std::cout << "Enter phone number: " << std::flush;
	  std::string phone_number;
	  std::cin >> phone_number;
	  send_query (
	    td_api::make_object<td_api::setAuthenticationPhoneNumber> (
	      phone_number, nullptr),
	    create_authentication_query_handler ());
	  break;
	}
	case td_api::authorizationStateWaitEmailAddress::ID: {
	  std::cout << "Enter email address: " << std::flush;
	  std::string email_address;
	  std::cin >> email_address;
	  send_query (
	    td_api::make_object<td_api::setAuthenticationEmailAddress> (
	      email_address),
	    create_authentication_query_handler ());
	  break;
	}
	case td_api::authorizationStateWaitEmailCode::ID: {
	  std::cout << "Enter email authentication code: " << std::flush;
	  std::string code;
	  std::cin >> code;
	  send_query (
	    td_api::make_object<td_api::checkAuthenticationEmailCode> (
	      td_api::make_object<td_api::emailAddressAuthenticationCode> (
		code)),
	    create_authentication_query_handler ());
	  break;
	}
	case td_api::authorizationStateWaitCode::ID: {
	  std::cout << "Enter authentication code: " << std::flush;
	  std::string code;
	  std::cin >> code;
	  send_query (td_api::make_object<td_api::checkAuthenticationCode> (
			code),
		      create_authentication_query_handler ());
	  break;
	}

	case td_api::authorizationStateWaitRegistration::ID: {
	  std::string first_name;
	  std::string last_name;
	  std::cout << "Enter your first name: " << std::flush;
	  std::cin >> first_name;
	  std::cout << "Enter your last name: " << std::flush;
	  std::cin >> last_name;
	  send_query (td_api::make_object<td_api::registerUser> (first_name,
								 last_name),
		      create_authentication_query_handler ());
	  break;
	}
	case td_api::authorizationStateWaitPassword::ID: {
	  std::cout << "Enter authentication password: " << std::flush;
	  std::string password;
	  std::getline (std::cin, password);
	  send_query (td_api::make_object<td_api::checkAuthenticationPassword> (
			password),
		      create_authentication_query_handler ());
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

	case td_api::authorizationStateWaitTdlibParameters::ID: {
	  std::string inbuf;

	  auto request = td_api::make_object<td_api::setTdlibParameters> ();
	  request->database_directory_ = tf_data.path_tddata ();
	  request->use_message_database_ = true;
	  request->use_secret_chats_ = true;
	  request->api_id_ = tf_data.get_api_id_as_int32 ();
	  request->api_hash_ = tf_data.get_api_hash ();
	  // request->api_id_ = api_id;	 // 11216842;
	  // request->api_hash_ = api_hash; //
	  // "a80e564e0c4d452a0874e8f4555c8e6c";
	  request->system_language_code_ = "en";
	  request->device_model_ = "Desktop";
	  request->application_version_ = "1.0";
	  request->enable_storage_optimizer_ = true;
	  send_query (std::move (request),
		      create_authentication_query_handler ());
	  break;
	}
      }
  }

  void check_authentication_error (Object object)
  {
    if (object->get_id () == td_api::error::ID)
      {
	auto error = td::move_tl_object_as<td_api::error> (object);
	std::cout << "Error: " << to_string (error) << std::flush;
	on_authorization_state_update ();
      }
  }

  std::uint64_t next_query_id () { return ++current_query_id_; }
};

// int
// main ()
// {
//   TdAuth example;
//   example.loop ();
// }

#endif
