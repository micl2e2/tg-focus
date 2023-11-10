#include "lv_log.hh"
#include "auth.hh"
#include "common.hh"
#include <td/telegram/td_api.h>

TdAuth::TdAuth ()
{
  td::ClientManager::execute (
    td_api::make_object<td_api::setLogVerbosityLevel> (1));
  client_manager_ = std::make_unique<td::ClientManager> ();
  client_id_ = client_manager_->create_client_id ();
  send_query (td_api::make_object<td_api::getOption> ("version"), {});
}

TdAuth::~TdAuth ()
{
  send_query (td_api::make_object<td_api::close> (), [] (Object obj) {
    if (obj->get_id () == td_api::ok::ID)
      lv_log (LogLv::INFO, "Closing...");
  });

  while (!is_tdlib_closed.load (std::memory_order_acquire))
    {
      auto response = client_manager_->receive (60);
      if (response.object)
	{
	  process_response (std::move (response));
	}
    }
}

void
TdAuth::loop ()
{
  while (!is_login.load (std::memory_order_acquire))
    {
      auto response = client_manager_->receive (60);
      if (response.object)
	{
	  process_response (std::move (response));
	}
    }
  lv_log (LogLv::INFO, "Log in successfully!");
}

void
TdAuth::send_query (td_api::object_ptr<td_api::Function> f,
		    std::function<void (Object)> handler)
{
  auto query_id = next_query_id ();
  if (handler)
    {
      handlers_.emplace (query_id, std::move (handler));
    }
  client_manager_->send (client_id_, query_id, std::move (f));
}

void
TdAuth::process_response (td::ClientManager::Response response)
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

std::string
TdAuth::get_user_name (std::int64_t user_id) const
{
  auto it = users_.find (user_id);
  if (it == users_.end ())
    {
      return "unknown user";
    }
  return it->second->first_name_ + " " + it->second->last_name_;
}

std::string
TdAuth::get_chat_title (std::int64_t chat_id) const
{
  auto it = chat_title_.find (chat_id);
  if (it == chat_title_.end ())
    {
      return "unknown chat";
    }
  return it->second;
}

void
TdAuth::process_update (td_api::object_ptr<td_api::Object> update)
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

auto
TdAuth::auth_query_callback ()
{
  return [this] (Object object) {
    if (object->get_id () == td_api::ok::ID)
      {
	// std::cout << "OK!" << std::endl;
      }
    else if (object->get_id () == td_api::error::ID)
      {
	auto error = td::move_tl_object_as<td_api::error> (object);

	if (error->message_.find ("PHONE_NUMBER_INVALID") != std::string::npos)
	  lv_log (LogLv::ERROR, "The phone number is invalid");
	else if (error->message_.find ("PHONE_CODE_HASH_EMPTY")
		 != std::string::npos)
	  lv_log (LogLv::ERROR, "phone_code_hash is missing");
	else if (error->message_.find ("PHONE_CODE_EMPTY") != std::string::npos)
	  lv_log (LogLv::ERROR, "phone_code is missing");
	else if (error->message_.find ("PHONE_CODE_EXPIRED")
		 != std::string::npos)
	  lv_log (LogLv::ERROR, "The confirmation code has expired");
	else if (error->message_.find ("PHONE_CODE_INVALID")
		 != std::string::npos)
	  lv_log (LogLv::ERROR, "The login code is invalid");
	else if (error->message_.find ("API_ID_INVALID") != std::string::npos)
	  lv_log (LogLv::ERROR, "The api_id/api_hash combination is invalid"
				"(restart might be needed)");
	else if (error->message_.find ("PASSWORD_HASH_INVALID")
		 != std::string::npos)
	  lv_log (LogLv::ERROR, "Incorrect password");
	else if (error->message_.find ("PHONE_NUMBER_UNOCCUPIED")
		 != std::string::npos)
	  lv_log (LogLv::ERROR, "The phone number is not yet being used");
	else if (error->message_.find (
		   "Valid api_id must be provided. Can be obtained at")
		 != std::string::npos)
	  lv_log (LogLv::ERROR, "Invalid API ID");
	else
	  {
	    lv_log (LogLv::ERROR, "ERROR: {}", error->message_);
	    lv_log (LogLv::ERROR, "Fatal errors. Please submit a bug report.");
	    std::exit (11);
	  }
	on_authorization_state_update ();
      }
    else
      {
	lv_log (LogLv::ERROR, "ERROR: unexpected auth callback id:{}",
		object->get_id ());
      }
  };
}

void
TdAuth::on_authorization_state_update ()
{
  switch (this->auth_state_->get_id ())
    {
      case td_api::authorizationStateReady::ID: {
	// this means: api id/hash correct, phone and vcode correct

	this->is_authorized = true;

	// persistant
	tf_data.set_api_id (fmt::format ("{}", this->api_id_));
	tf_data.set_api_hash (std::move (this->api_hash_));
	tf_data.set_auth_hint (true);

	is_login.store (true, std::memory_order_release);

	break;
      }

      // FIXME:  auth-reset should make use of this event
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
	lv_log (LogLv::INFO, "Closed");
	this->is_authorized = false;
	is_tdlib_closed.store (true, std::memory_order_release);
	break;
      }

      case td_api::authorizationStateWaitPhoneNumber::ID: {
	log_flush ("Enter phone number: ");
	std::string phone_number;
	std::cin >> phone_number;
	send_query (td_api::make_object<td_api::setAuthenticationPhoneNumber> (
		      phone_number, nullptr),
		    auth_query_callback ());
	break;
      }

      case td_api::authorizationStateWaitCode::ID: {
	log_flush ("Enter authentication code: ");
	std::string code;
	std::cin >> code;
	send_query (td_api::make_object<td_api::checkAuthenticationCode> (code),
		    auth_query_callback ());
	break;
      }

      case td_api::authorizationStateWaitPassword::ID: {
	log_flush ("Enter authentication password: ");
	std::string code;
	std::cin >> code;
	send_query (td_api::make_object<td_api::checkAuthenticationPassword> (
		      code),
		    auth_query_callback ());
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
	    std::cin >> may_api_id;

	    if (is_valid_int32 (may_api_id))
	      api_id = sstr_to_int32 (may_api_id);
	    else
	      api_id = 0;

	    log_flush ("Enter api hash: ");
	    std::cin >> api_hash;
	  }

	auto request = td_api::make_object<td_api::setTdlibParameters> ();
	request->database_directory_ = tf_data.path_tddata ();
	request->use_message_database_ = true;
	request->use_secret_chats_ = true;
	request->api_id_ = api_id;
	request->api_hash_ = api_hash;
	request->system_language_code_ = "en";
	request->device_model_ = TF_DEV;
	request->application_version_ = TF_VER;
	request->enable_storage_optimizer_ = true;

	this->api_id_ = api_id;
	this->api_hash_ = api_hash;

	send_query (std::move (request), auth_query_callback ());
	break;
      }

      default: {
	lv_log (LogLv::ERROR, "ERROR: unknown event {}",
		this->auth_state_->get_id ());
	lv_log (LogLv::ERROR, "Fatal errors. Please submit a bug report.");
	std::exit (10);
      }
    }
}
