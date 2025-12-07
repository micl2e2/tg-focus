#define __TU__ "td_auth"

#include "stat.hh"
#include "log.hh"
#include "td_auth.hh"
#include "std_comp.hh"
#include "common.hh"

tgf::TdAuth::TdAuth (bool useProvidedApiPass)
{
  __clientid = 0;
  __curr_qry_id = 0;
  __apiid = 0;
  __apihash = ""s;
  td::ClientManager::execute (tapi_mkobj<tapi::setLogVerbosityLevel> (1));
  __client = make_unique<td::ClientManager> ();
  __clientid = __client->create_client_id ();
  send_query (tapi_mkobj<tapi::getOption> ("version"), {});
  send_query (tapi_mkobj<tapi::setOption> ("use_storage_optimizer", tapi_mkobj<tapi::optionValueBoolean> (true)), {});
  __use_our_api = useProvidedApiPass;
}

tgf::TdAuth::~TdAuth ()
{
  send_query (tapi_mkobj<tapi::close> (), [] (TdObjPtr obj) {
    if (obj->get_id () == tapi::ok::ID)
      tgf::logd ("Closing...");
  });

  while (!tgfstat::p::is_tdlib_closed.load (memory_order_acquire))
    {
      auto response = __client->receive (60);
      if (response.object)
	{
	  process_response (move (response));
	}
    }
}

void
tgf::TdAuth::loop ()
{
  while (!tgfstat::p::is_login.load (memory_order_acquire))
    {
      td::ClientManager::Response response = __client->receive (60);
      if (response.object)
	{
	  process_response (move (response));
	}
    }
  cout << ("Logged in!") << endl;
}

void
tgf::TdAuth::send_query (tapi::object_ptr<tapi::Function> tdfn, function<void (TdObjPtr)> cbfn)
{
  auto query_id = next_query_id ();
  if (cbfn)
    {
      __cb.emplace (query_id, move (cbfn));
    }
  tulogfd_cg (1, 232323);
  __client->send (__clientid, query_id, move (tdfn));
}

void
tgf::TdAuth::process_response (td::ClientManager::Response response)
{
  if (!response.object)
    return;
  if (response.request_id == 0)
    return process_update (move (response.object));
  auto it = __cb.find (response.request_id);
  if (it != __cb.end ())
    {
      it->second (move (response.object));
      __cb.erase (it);
    }
}

void
tgf::TdAuth::process_update (TdObjPtr update)
{
  switch (update->get_id ())
    {
      case tapi::updateAuthorizationState::ID: {
	tapi::updateAuthorizationState *casted = static_cast<tapi::updateAuthorizationState *> (update.get ());
	this->__auth_stat = move (casted->authorization_state_);
	this->on_authorization_state_update ();
	break;
      }
    }
}

function<void (TdObjPtr)>
tgf::TdAuth::auth_query_callback ()
{
  return [this] (TdObjPtr object) {
    if (object->get_id () == tapi::ok::ID)
      {
	// cout << "OK!" << endl;
      }
    else if (object->get_id () == tapi::error::ID)
      {
	auto error = tapi_movas<tapi::error> (object);

	if (error->message_.find ("PHONE_NUMBER_INVALID") != string::npos)
	  tgf::loge ("The phone number is invalid");
	else if (error->message_.find ("PHONE_CODE_HASH_EMPTY") != string::npos)
	  tgf::loge ("phone_code_hash is missing");
	else if (error->message_.find ("PHONE_CODE_EMPTY") != string::npos)
	  tgf::loge ("phone_code is missing");
	else if (error->message_.find ("PHONE_CODE_EXPIRED") != string::npos)
	  tgf::loge ("The confirmation code has expired");
	else if (error->message_.find ("PHONE_CODE_INVALID") != string::npos)
	  tgf::loge ("The login code is invalid");
	else if (error->message_.find ("API_ID_INVALID") != string::npos)
	  tgf::loge ("The api_id/api_hash combination is invalid"
		     "(restart might be needed)");
	else if (error->message_.find ("PASSWORD_HASH_INVALID") != string::npos)
	  tgf::loge ("Incorrect password");
	else if (error->message_.find ("PHONE_NUMBER_UNOCCUPIED") != string::npos)
	  tgf::loge ("The phone number is not yet being used");
	else if (error->message_.find ("Valid api_id must be provided. Can be obtained at") != string::npos)
	  tgf::loge ("Invalid API ID");
	else
	  {
	    tgf::loge ("ERROR: {}", error->message_);
	    tgf::loge ("Fatal errors. Please submit a bug report.");
	    exit (11);
	  }
	on_authorization_state_update ();
      }
    else
      {
	tgf::loge ("ERROR: unexpected auth callback id:{}", object->get_id ());
      }
  };
}

void
tgf::TdAuth::on_authorization_state_update ()
{
  switch (this->__auth_stat->get_id ())
    {
      case tapi::authorizationStateReady::ID: {
	// api id/hash correct, phone and vcode correct

	this->__is_auth = true;

	// persistent
	tgfstat::userdata.set_api_id (this->__apiid);
	tgfstat::userdata.set_api_hash (move (this->__apihash));
	tgfstat::userdata.set_auth_hint (true);

	tgfstat::p::is_login.store (true, memory_order_release);

	break;
      }

      // FIXME:  auth-reset should make use of this event
      // case tapi::AuthorizationStateLogOut::ID: {
      //   this->__is_auth = false;
      //   cout << "Logging out" << endl;
      //   break;
      // }

      case tapi::authorizationStateClosing::ID: {
	// cout << "Closing" << endl;
	break;
      }

      case tapi::authorizationStateClosed::ID: {
	tgf::logd ("Closed");
	this->__is_auth = false;
	tgfstat::p::is_tdlib_closed.store (true, memory_order_release);
	break;
      }

      case tapi::authorizationStateWaitPhoneNumber::ID: {
	cout << ("Enter phone number: ") << flush;
	string phone_number;
	getline (cin, phone_number);
	string phone_number_fotmatted = rmspc (move (phone_number));

	send_query (tapi_mkobj<tapi::setAuthenticationPhoneNumber> (phone_number_fotmatted, nullptr), auth_query_callback ());
	break;
      }

      case tapi::authorizationStateWaitCode::ID: {
	cout << ("Enter login code: ") << flush;
	string code;
	cin >> code;
	send_query (tapi_mkobj<tapi::checkAuthenticationCode> (code), auth_query_callback ());
	break;
      }

      case tapi::authorizationStateWaitPassword::ID: {
	cout << ("Enter authentication password: ") << flush;
	string code;
	cin >> code;
	send_query (tapi_mkobj<tapi::checkAuthenticationPassword> (code), auth_query_callback ());
	break;
      }

      // assuming always the first event
      case tapi::authorizationStateWaitTdlibParameters::ID: {
	int32_t api_id;
	string may_api_id;
	string api_hash;

	if (tgfstat::userdata.get_auth_hint ())
	  {
	    api_id = tgfstat::userdata.get_api_id ();
	    api_hash = tgfstat::userdata.get_api_hash ();
	  }
	else if (this->__use_our_api)
	  {
	    int p1 = 0x01b6;
	    int p2 = 0x7787;
	    api_id = p1 << 0x10 | p2;
	    api_hash = "\x35\x65\x34\x64\x30\x31\x31\x65\x62\x64\x31\x34\x39"
		       "\x61\x34\x31\x38\x61\x35\x61\x31\x62\x34\x64\x35\x35"
		       "\x64\x35\x33\x66\x37\x30";
	  }
	else
	  {
	    cout << ("Enter api id: ") << flush;
	    cin >> may_api_id;

	    if (is_valid_int32 (may_api_id))
	      api_id = sstr_to_int32 (may_api_id);
	    else
	      api_id = 0;

	    cout << ("Enter api hash: ") << flush;
	    cin >> api_hash;
	  }

	auto request = tapi_mkobj<tapi::setTdlibParameters> ();
	request->database_directory_ = tgfstat::userdata.path_tddata ();
	request->use_message_database_ = true;
	request->use_secret_chats_ = true;
	request->api_id_ = api_id;
	request->api_hash_ = api_hash;
	request->system_language_code_ = "en";
	request->device_model_ = TF_DEV;
	request->application_version_ = TF_VER;

	this->__apiid = api_id;
	this->__apihash = api_hash;

	send_query (move (request), auth_query_callback ());
	break;
      }

      default: {
	tgf::loge ("ERROR: unknown event {}", this->__auth_stat->get_id ());
	tgf::loge ("Fatal errors. Please submit a bug report.");
	exit (10);
      }
    }
}
