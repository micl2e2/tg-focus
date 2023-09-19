#ifndef _TD_CLIENT_H
#define _TD_CLIENT_H

#include <cstdlib>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>
#include <map>
#include <functional>
#include <atomic>
#include <format>
#include <chrono>
#include <iostream>

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include "producer.hh"
#include "tg_focus_data.hh"

#include "tgfocus_state.hh"
extern std::vector<TgMsg> mq;
extern std::mutex mq_lock;
extern std::atomic<bool> is_csm_mq;
extern TgFocusData tf_data;
extern std::atomic<std::uint32_t> it_cnt_producer;
extern std::atomic<std::uint32_t> it_cnt_consumer;

namespace td_api = td::td_api;

class TdClient
{
public:
  TdClient () = default;

  bool tried_create_collector{false};
  bool done_create_collector{false};

  //
  // initialize td client
  void init ()
  {
    td::ClientManager::execute (
      td_api::make_object<td_api::setLogVerbosityLevel> (1));
    client_manager_ = std::make_unique<td::ClientManager> ();
    client_id_ = client_manager_->create_client_id ();
    send_query (td_api::make_object<td_api::getOption> ("version"), {});
  }

  //
  // create collector chat
  void create_tgfocus_group ()
  {
    // the handle call time maybe a little late, one proved order:
    /*
no message!
no message!
Authorization is completed
no message!
no message!
...
   msg:xxx
   left: 0
   consume_cnt: 91
...
   msg:xxx
   left: 0
   consume_cnt: 97
...
   msg:xxx
   left: 0
   consume_cnt: 100
...
no message!
no message!
group created!  chat id:-4078504482 chat title:TG-FOCUS
no message!
no message!
...
     */
    if (this->is_authorized && !this->tried_create_collector
	&& !this->done_create_collector)
      {
	this->tried_create_collector = true;
	send_query (td_api::make_object<td_api::createNewBasicGroupChat> (
		      std::vector<td::td_api::int53> (0), "TG-FOCUS", 0),
		    [this] (Object object) {
		      if (object->get_id () == td_api::chat::ID)
			{
			  auto chat
			    = td::move_tl_object_as<td_api::chat> (object);
			  // std::cout << "group created! "
			  // 	    << " chat id:" << chat->id_
			  // 	    << " chat title:" << chat->title_
			  // 	    << std::endl;
			  this->collector_id = chat->id_;
			  this->done_create_collector = true;
			}
		    });
      }
  }

  //
  // send message to collector chat
  void collect_msg (const TgMsg &msg, size_t c_count)
  {
    // https://core.telegram.org/tdlib/docs/td__api_8h.html#a7b249263de52128c32781ba0e713b556
    // if (msg.get_chat_title ().find ("TG-FOCUS") != std::string::npos)
    //   {
    // 	return;
    //   }

    auto text_ctn
      = std::format (R"(
ID : {}
CHAT : {}
SENDER : {}
CONTENT: {}
DATE: {}
)",
		     c_count, msg.get_chat_title (), msg.get_sender (),
		     msg.get_text_content (), msg.get_timestamp ());
    auto message_text = td_api::make_object<td_api::formattedText> (
      text_ctn, td_api::array<td_api::object_ptr<td_api::textEntity>> ());
    td_api::object_ptr<td_api::Function> send_message_request
      = td_api::make_object<td_api::sendMessage> (
	this->collector_id, 0, nullptr, nullptr, nullptr,
	td_api::make_object<td_api::inputMessageText> (std::move (message_text),
						       false, true));

    send_query (std::move (send_message_request), [this, msg] (Object object) {
      if (object->get_id () == td_api::message::ID)
	{
	  // FIXME: do not use operator <<
	  std::cout << std::format ("[CONSUMER {}] msg collected:",
				    it_cnt_consumer.load (
				      std::memory_order_relaxed))
		    << msg << std::endl;
	}
    });
  }

  //
  // signal td client to fetch updates
  void fetch_updates ()
  {
    auto response = client_manager_->receive (60);
    if (response.object)
      {
	std::cerr << std::format ("[PRODUCER {}] td-client, resp recv id:{}",
				  it_cnt_producer.load (
				    std::memory_order_relaxed),
				  response.object->get_id ())
		  << std::endl;
	process_response (std::move (response));
      }
  }

  bool is_authorized{false};

private:
  using Object = td_api::object_ptr<td_api::Object>;
  std::unique_ptr<td::ClientManager> client_manager_; // client manager obj
  std::int32_t client_id_{0};			      // client id

  td_api::object_ptr<td_api::AuthorizationState> auth_state_;
  td::td_api::int53 collector_id{0};
  std::uint64_t current_query_id_{0}; // increase by each query
  std::uint64_t authentication_query_id_{0};

  std::map<std::uint64_t, std::function<void (Object)>> handlers_;

  std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;

  std::map<std::int64_t, std::string> chat_title_;

  //
  // signal td client to send a request
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

  //
  // process response from td client, maybe for an automatic update, maybe for a
  // user's request.
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
	std::cerr << std::format (
	  "[PRODUCER {}]td-client, handlers_.size():{} it->first:{}",
	  it_cnt_producer.load (std::memory_order_relaxed), handlers_.size (),
	  it->first)
		  << std::endl;
	it->second (std::move (response.object));
	handlers_.erase (it);
      }
  }

  //
  // get user name from internal map
  std::string get_user_name (std::int64_t user_id) const
  {
    auto it = this->users_.find (user_id);
    if (it == users_.end ())
      {
	return "unknown user";
      }

    auto readable_usrname
      = it->second->first_name_ + " " + it->second->last_name_;

    if (it->second->usernames_)
      {
	readable_usrname += "@" + it->second->usernames_->editable_username_;
      }

    return readable_usrname;
  }

  //
  // get chat title from internal map
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
	  auto casted = static_cast<updateChatTitle *> (update.get ());
	  this->chat_title_[casted->chat_id_] = casted->title_;
	  break;
	}

	case updateUser::ID: {
	  auto casted = static_cast<updateUser *> (update.get ());
	  auto user_id = casted->user_->id_;
	  this->users_[user_id] = std::move (casted->user_);
	  break;
	}

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
	  switch (casted->message_->content_->get_id ())
	    {
	      case td_api::messageText::ID: {
		auto orig_txt_ctn = static_cast<td_api::messageText &> (
				      *casted->message_->content_)
				      .text_->text_;
		text = std::move (orig_txt_ctn);
		break;
	      }

	      case td_api::messagePhoto::ID: {
		auto orig_txt_ctn = static_cast<td_api::messagePhoto &> (
				      *casted->message_->content_)
				      .caption_->text_;
		text = std::format ("<photo>({})", std::move (orig_txt_ctn));
		break;
	      }

	      case td_api::messageAnimatedEmoji::ID: {
		auto orig_txt_ctn
		  = static_cast<td_api::messageAnimatedEmoji &> (
		      *casted->message_->content_)
		      .emoji_;
		text = std::format ("<emoji>({})", std::move (orig_txt_ctn));
		break;
	      }

	      case td_api::messageSticker::ID: {
		auto orig_txt_ctn = static_cast<td_api::messageSticker &> (
				      *casted->message_->content_)
				      .sticker_->emoji_;
		text = std::format ("<sticker>({})", std::move (orig_txt_ctn));
		break;
	      }

	      default: {
		text = std::format ("ignored message with ID {}",
				    casted->message_->content_->get_id ());
		break;
	      }
	    }

	  std::int32_t tstamp = casted->message_->date_;

	  std::lock_guard<std::mutex> mq_guard (mq_lock);
	  mq.insert (mq.begin (),
		     TgMsg (chat_title_[chat_id], sender_name, text, tstamp));

	  break;
	}

	default: {
	  std::cerr << std::format (
	    "[PRODUCER {}] td-client, ignored update with id:{}",
	    it_cnt_producer.load (std::memory_order_relaxed), update->get_id ())
		    << std::endl;
	  break;
	}
      }
  }

  auto auth_query_callback ()
  {
    return [this, id = authentication_query_id_] (Object object) {
      if (id == authentication_query_id_)
	{
	  // FIXME: logging
	  std::cerr << "create auth query handle callback1, id:";
	  if (object->get_id () == td_api::error::ID)
	    {
	      auto error = td::move_tl_object_as<td_api::error> (object);
	      std::cerr << "Error: " << to_string (error) << std::flush;
	    }
	}
    };
  }

  void on_authorization_state_update ()
  {
    this->authentication_query_id_++;

    switch (this->auth_state_->get_id ())
      {
	case td_api::authorizationStateReady::ID: {
	  this->is_authorized = true;
	  std::cout << "Authorization is completed" << std::endl;
	  break;
	}

	case td_api::authorizationStateWaitTdlibParameters::ID: {
	  std::cerr << "wait tdlib para... " << std::endl;

	  std::string inbuf;

	  auto request = td_api::make_object<td_api::setTdlibParameters> ();
	  request->database_directory_ = tf_data.path_tddata ();
	  request->use_message_database_ = true;
	  request->use_secret_chats_ = true;
	  request->api_id_ = tf_data.get_api_id_as_int32 ();
	  request->api_hash_ = tf_data.get_api_hash (); //
	  request->system_language_code_ = "en";
	  request->device_model_ = "Desktop";
	  request->application_version_ = "1.0";
	  request->enable_storage_optimizer_ = true;
	  send_query (std::move (request), auth_query_callback ());
	  break;
	}

	default: {
	  std::cerr << "ignored auth state with id "
		    << this->auth_state_->get_id () << std::endl;
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
      }
    else
      {
	std::cerr << "check auth no error, id:" << object->get_id ()
		  << std::endl;
      }
  }

  //
  // td request id
  std::uint64_t next_query_id () { return ++current_query_id_; }
};

#endif
