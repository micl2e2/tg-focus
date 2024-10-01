#include <cuchar>
#include <cstddef>
#include <cstdlib>
#include <tuple>
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

#include "common.hh"
#include "lv_log.hh"
#include "tgf_data.hh"
#include "collector.hh"

extern std::vector<TgMsg> mq;
extern std::mutex mq_lock;
extern std::atomic<bool> is_csm_mq;
extern TgFocusData tgf_data;
extern std::atomic<std::uint32_t> it_cnt_producer;
extern std::atomic<std::uint32_t> it_cnt_consumer;

namespace td_api = td::td_api;

std::ostream &
operator<< (std::ostream &os, std::tuple<int, int> t)
{
  os << "<" << get<0> (t) << "," << get<1> (t) << ">";

  return os;
}

template <typename T>
std::ostream &
operator<< (std::ostream &os, std::vector<T> t)
{
  for (auto ele : t)
    os << ele << ",";

  return os;
}

// public

void
TdCollector::init ()
{
  td::ClientManager::execute (
    td_api::make_object<td_api::setLogVerbosityLevel> (1));
  client_manager_ = std::make_unique<td::ClientManager> ();
  client_id_ = client_manager_->create_client_id ();
  send_query (td_api::make_object<td_api::getOption> ("version"), {});
  send_query (td_api::make_object<td_api::setOption> (
		"use_storage_optimizer",
		td_api::make_object<td_api::optionValueBoolean> (true)),
	      {});
}

void
TdCollector::try_create_tgfchat () // FIXME: is try
{
  if (this->is_authorized && !tgf_data.is_tgfid_valid ()
      && !this->tried_create_tgfchat)
    {
      this->tried_create_tgfchat = true;

      if (tgf_data.is_super_tgfid ())
	{
	  send_query (
	    td_api::make_object<td_api::createNewSupergroupChat> (
	      TF_COLL_CHAT_TITLE, false, false, "TG-FOCUS helps you focus!",
	      nullptr, 0, false),
	    [this] (Object object) {
	      if (object->get_id () == td_api::chat::ID)
		{
		  auto chat = td::move_tl_object_as<td_api::chat> (object);
		  lvlog (LogLv::INFO, "group created", " chat id:", chat->id_);
		  tgf_data.set_tgfid (static_cast<int64_t> (chat->id_));
		}
	      else if (object->get_id () == td_api::error::ID)
		{
		  auto error = td::move_tl_object_as<td_api::error> (object);
		  lvlog (LogLv::ERROR, "error code:", error->code_,
			 " error message:", error->message_);
		}
	      else
		{
		  lvlog (LogLv::ERROR, "unexpected tlobj:", object->get_id ());
		}
	    });
	}
      else
	{
	  send_query (
	    td_api::make_object<td_api::createNewBasicGroupChat> (
	      std::vector<td::td_api::int53> (0), TF_COLL_CHAT_TITLE, 0),
	    [this] (Object object) {
	      if (object->get_id () == td_api::createdBasicGroupChat::ID)
		{
		  auto chat
		    = td::move_tl_object_as<td_api::createdBasicGroupChat> (
		      object);
		  lvlog (LogLv::INFO, "group created",
			 " chat id:", chat->chat_id_);
		  tgf_data.set_tgfid (static_cast<int64_t> (chat->chat_id_));
		}
	      else
		{
		  lvlog (LogLv::ERROR, "unexpected tlobj:", object->get_id ());
		}
	    });
	}
    }
}

td_api::array<td_api::object_ptr<td_api::textEntity>>
decorate_msg (const std::string &msg)
{
  // return td_api::array<td_api::object_ptr<td_api::textEntity>>{};

  auto pos_info = get_decor_pos (msg);

  // FIXME: only when very verbose
  lvlog (LogLv::DEBUG,
	 "consumer_cnt:", it_cnt_consumer.load (std::memory_order_relaxed),
	 " decorating u8str:", msg, " pos_info:", decor_pos_to_str (pos_info));

  auto deco_list = td_api::array<td_api::object_ptr<td_api::textEntity>> ();

  for (auto pos : pos_info)
    {
      deco_list.emplace_back (td_api::make_object<td_api::textEntity> (
	std::get<0> (pos), std::get<1> (pos),
	td_api::make_object<td_api::textEntityTypeBold> ()));
    }

  return deco_list;
}

void
TdCollector::collect_msg (const TgMsg &msg)
{
  std::string tfmsg_str = msg.to_locale_string ();

  auto text_deco_list = decorate_msg (tfmsg_str);

  auto message_text
    = td_api::make_object<td_api::formattedText> (tfmsg_str,
						  std::move (text_deco_list));

  td_api::object_ptr<td_api::Function> send_message_request
    = td_api::make_object<td_api::sendMessage> (
      // this->collector_id, //
      tgf_data.get_tgfid (), //
      0, nullptr, nullptr, nullptr,
      td_api::make_object<td_api::inputMessageText> (std::move (message_text),
						     nullptr, true));

  send_query (std::move (send_message_request), [this, msg] (Object object) {
    if (object->get_id () == td_api::message::ID)
      {
	// FIXME: do not use operator <<
	lvlog (LogLv::INFO, "consumer_cnt:",
	       it_cnt_consumer.load (std::memory_order_relaxed),
	       " msg collected:", msg.to_string ());
      }
    else if (object->get_id () == td_api::error::ID)
      {
	auto error = td::move_tl_object_as<td_api::error> (object);
	lvlog (LogLv::ERROR, "msg not collected", " error code:", error->code_,
	       " error message:", error->message_);
	if (error->message_.find ("Have no write access to the chat")
	      != std::string::npos
	    || error->message_.find ("Chat not found") != std::string::npos)
	  {
	    this->tried_create_tgfchat = false;
	    tgf_data.set_tgfid (-1);
	  }
      }
  });
}

void
TdCollector::fetch_updates ()
{
  auto response = client_manager_->receive (60);
  if (response.object)
    {
      lvlog (LogLv::DEBUG,
	     "producer_iter:", it_cnt_producer.load (std::memory_order_relaxed),
	     " td-client, resp recv id:",

	     response.object->get_id ());
      process_response (std::move (response));
    }
}

// private

void
TdCollector::send_query (td_api::object_ptr<td_api::Function> f,
			 std::function<void (Object)> handler)
{
  auto query_id = next_query_id ();
  lvlog (LogLv::DEBUG, "TdCollector::send_query !!!");
  if (handler)
    {
      handlers_.emplace (query_id, std::move (handler));
    }
  client_manager_->send (client_id_, query_id, std::move (f));
}

void
TdCollector::process_response (td::ClientManager::Response response)
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
      lvlog (LogLv::DEBUG,
	     "producer_iter:", it_cnt_producer.load (std::memory_order_relaxed),
	     " handlers_.size():", handlers_.size (), " it->first:", it->first);

      it->second (std::move (response.object));
      handlers_.erase (it);
    }
}

std::string
TdCollector::get_user_name (std::int64_t user_id) const
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

std::string
TdCollector::get_chat_title (std::int64_t chat_id) const
{
  auto it = chat_title_.find (chat_id);
  if (it == chat_title_.end ())
    {
      return "unknown chat";
    }
  return it->second;
}

void
TdCollector::process_update (td_api::object_ptr<td_api::Object> update)
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
	      auto casted = static_cast<messageSenderUser *> (sender_id.get ());
	      sender_name = get_user_name (casted->user_id_);
	      break;
	    }
	    case messageSenderChat::ID: {
	      auto casted = static_cast<messageSenderChat *> (sender_id.get ());
	      sender_name += get_chat_title (casted->chat_id_);
	      sender_name += "(chat)";
	      // sender_name
	      // "{}(chat)"
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

	      text += "<photo>";
	      text += "(";
	      text += std::move (orig_txt_ctn);
	      text += ")";
	      // "<photo>({})"
	      break;
	    }

	    case td_api::messageAnimatedEmoji::ID: {
	      auto orig_txt_ctn = static_cast<td_api::messageAnimatedEmoji &> (
				    *casted->message_->content_)
				    .emoji_;
	      text += "<emoji>";
	      text += "(";
	      text += std::move (orig_txt_ctn);
	      text += ")";
	      // "<emoji>({})"
	      break;
	    }

	    case td_api::messageSticker::ID: {
	      auto orig_txt_ctn = static_cast<td_api::messageSticker &> (
				    *casted->message_->content_)
				    .sticker_->emoji_;
	      text += "<sticker>";
	      text += "(";
	      text += std::move (orig_txt_ctn);
	      text += ")";
	      // "<sticker>({})"
	      break;
	    }

	    default: {
	      text += "ignored message with ID ";
	      text += casted->message_->content_->get_id ();
	      // "ignored message with ID {}"
	      break;
	    }
	  }

	std::int32_t tstamp = casted->message_->date_;

	// ---
	std::lock_guard<std::mutex> mq_guard (mq_lock);

	TgMsg msg (chat_title_[chat_id], sender_name, text, tstamp);

	if (!msg.is_from_tgfocus ())
	  mq.insert (mq.begin (), std::move (msg));

	break;
      }

      default: {
	lvlog (LogLv::DEBUG, "producer_iter:",
	       it_cnt_producer.load (std::memory_order_relaxed),
	       " ignored update with id:", update->get_id ());
	break;
      }
    }
}

auto
TdCollector::auth_query_callback ()
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

void
TdCollector::on_authorization_state_update ()
{
  this->authentication_query_id_++;

  switch (this->auth_state_->get_id ())
    {
      case td_api::authorizationStateReady::ID: {
	this->is_authorized = true;
	lvlog (LogLv::DEBUG, "Authorization is completed");
	break;
      }

      case td_api::authorizationStateWaitTdlibParameters::ID: {
	lvlog (LogLv::DEBUG, "wait tdlib para... ");

	std::string inbuf;

	auto request = td_api::make_object<td_api::setTdlibParameters> ();
	request->database_directory_ = tgf_data.path_tddata ();
	request->use_message_database_ = true;
	request->use_secret_chats_ = true;
	request->api_id_ = tgf_data.get_api_id ();
	request->api_hash_ = tgf_data.get_api_hash (); //
	request->system_language_code_ = "en";
	request->device_model_ = TF_DEV;
	request->application_version_ = TF_VER;
	send_query (std::move (request), auth_query_callback ());
	break;
      }

      default: {
	int32_t authState = this->auth_state_->get_id ();
	if (authState == td_api::authorizationStateWaitPhoneNumber::ID)
	  {
	    lvlog (
	      LogLv::ERROR,
	      "Logged in information is "
	      "corrupted. Run commands one by one: "
	      "`auth-reset`, `auth`, to resolve the issue. Please report the "
	      "issue if this message still exists.");
	    std::exit (2);
	  }
	else
	  {
	    lvlog (LogLv::ERROR, "ignored auth state with id ", authState);
	  }
	break;
      }
    }
}

void
TdCollector::check_authentication_error (Object object)
{
  if (object->get_id () == td_api::error::ID)
    {
      auto error = td::move_tl_object_as<td_api::error> (object);
      lvlog (LogLv::ERROR, to_string (error));
      // std::cout << "Error: " << to_string (error) << std::flush;
    }
  else
    {
      std::cerr << "check auth no error, id:" << object->get_id () << std::endl;
    }
}

std::uint64_t
TdCollector::next_query_id ()
{
  return ++current_query_id_;
}
