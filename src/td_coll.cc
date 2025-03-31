#define __TU__ "td_coll"

#include <algorithm>
#include <cuchar>
#include <cstddef>
#include <cstdlib>
#include <tuple>
#include <vector>
#include <atomic>
#include <mutex>
#include <map>
#include <functional>
#include <chrono>
#include <iostream>

#include "std_comp.hh"
#include "td_comp.hh"
#include "common.hh"
#include "log.hh"
#include "td_coll.hh"
#include "stat.hh"
#include "tg_msg.hh"
#include "filter.hh"
#include "chatcmd.hh"
#include "utfutils.hh"

void
TdCollector::init ()
{
  TdClient::execute (td_mkobj<TdSetLogLevel> (1));
  __client = make_unique<TdClient> ();
  __clientid = __client->create_client_id ();
  send_query (td_mkobj<TdGetOpt> ("version"), {});
  send_query (td_mkobj<TdSetOpt> ("use_storage_optimizer",
				  td_mkobj<TdOptValBool> (true)),
	      {});
}

void
TdCollector::try_create_tgfchat () // FIXME: is try
{
  if (this->is_auth && !tgfstat::userdata.is_tgfid_valid ()
      && !this->tried_c_tgfchat)
    {
      this->tried_c_tgfchat = true;

      if (tgfstat::userdata.is_super_tgfid ())
	{
	  send_query (
	    td_mkobj<TdCreatSuperChat> (TF_COLL_CHAT_TITLE, false, false,
					"TG-FOCUS helps you focus!", nullptr, 0,
					false),
	    [this] (TdObjPtr object) {
	      if (object->get_id () == TdChat::ID)
		{
		  auto chat = tl_movas<TdChat> (object);
		  tgf::logfi_cg (1, "group created", " chat id:", chat->id_);
		  tgfstat::userdata.set_tgfid (
		    static_cast<int64_t> (chat->id_));
		}
	      else if (object->get_id () == TdErr::ID)
		{
		  auto error = tl_movas<TdErr> (object);
		  tulogfe_cg (1, "error code:", error->code_,
			      " error message:", error->message_);
		}
	      else
		{
		  tulogfe_cg (1, "unexpected tlobj:", object->get_id ());
		}
	    });
	}
      else
	{
	  send_query (td_mkobj<TdCreatBasicChat> (vector<TdInt> (0),
						  TF_COLL_CHAT_TITLE, 0),
		      [this] (TdObjPtr object) {
			if (object->get_id () == TdBasicChat::ID)
			  {
			    auto chat = tl_movas<TdBasicChat> (object);
			    tgf::logfi_cg (1, "coll chat created",
					   " chat id:", chat->chat_id_);
			    tgfstat::userdata.set_tgfid (
			      static_cast<int64_t> (chat->chat_id_));
			  }
			else
			  {
			    tulogfe_cg (1,
					"unexpected tlobj:", object->get_id ());
			  }
		      });
	}
    }
}

TdArray<TdPtr<TdTxtEnt>>
decorate_msg (const string &msg)
{
  return TdArray<TdPtr<TdTxtEnt>>{};

  auto pos_info = tgf::get_decor_pos (msg);

  // only when very verbose
  tgf::logfd_cg (1, 66987, tgfstat::c::d::it_cnt_consumer.load (mo::relaxed),
		 msg, tgf::decor_pos_to_str (pos_info));

  TdArray<TdPtr<TdTxtEnt>> deco_list;

  for (auto pos : pos_info)
    {
      deco_list.emplace_back (td_mkobj<TdTxtEnt> (get<0> (pos), get<1> (pos),
						  td_mkobj<TdTxtEntBold> ()));
    }

  return deco_list;
}

void
TdCollector::collect_msg (const tgf::TgMsg &msg)
{
  string tfmsg_str = msg.to_locale_string ();

  TdArray<TdPtr<TdTxtEnt>> text_deco_list = decorate_msg (tfmsg_str);

  TdPtr<TdFmtTxt> message_text
    = td_mkobj<TdFmtTxt> (tfmsg_str, move (text_deco_list));

  TdPtr<TdFunc> send_message_request = td_mkobj<TdSendMsg> (
    // this->collector_id, //
    tgfstat::userdata.get_tgfid (), //
    0, nullptr, nullptr, nullptr,
    td_mkobj<TdInMsgTxt> (move (message_text), nullptr, true));

  send_query (move (send_message_request), [this, msg] (TdObjPtr object) {
    if (object->get_id () == TdMsg::ID)
      {
	tgf::logfd_cg (1, 888333,
		       tgfstat::c::d::it_cnt_consumer.load (mo::relaxed),
		       msg.to_string ());

	tgfstat::c::d::it_msg_coll.fetch_add (1, mo::relaxed);
      }
    else if (object->get_id () == TdErr::ID)
      {
	auto error = tl_movas<TdErr> (object);
	tulogfe_cg (1, "msg not collected", " error code:", error->code_,
		    " error message:", error->message_);
	if (error->message_.find ("Have no write access to the chat")
	      != string::npos
	    || error->message_.find ("Chat not found") != string::npos)
	  {
	    this->tried_c_tgfchat = false;
	    tgfstat::userdata.set_tgfid (-1);
	  }
      }
  });
}

void
TdCollector::fetch_updates ()
{
  auto response = __client->receive (10);
  if (response.object)
    {
      tgf::logfd_cg (1, "producer_iter:",
		     tgfstat::c::d::it_cnt_producer.load (mo::relaxed),
		     " td-client, resp recv id:",

		     response.object->get_id ());
      process_response (move (response));
    }
}

// private

void
TdCollector::send_query (TdPtr<TdFunc> tdfn, function<void (TdObjPtr)> cbfn)
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
TdCollector::process_response (TdClient::Response response)
{
  if (!response.object)
    return;
  if (response.request_id == 0)
    return process_update (move (response.object));
  auto it = __cb.find (response.request_id);
  if (it != __cb.end ())
    {
      tgf::logfd_cg (1, 881, tgfstat::c::d::it_cnt_producer.load (mo::relaxed),
		     __cb.size (), it->first);
      it->second (move (response.object));
      tulogfi_cg (1, 65776, __cb.size ());
      __cb.erase (it);
      tulogfi_cg (1, 65777, __cb.size ());
    }
}

string
TdCollector::get_user_name (int64_t user_id) const
{
  auto it = this->__users.find (user_id);
  if (it == __users.end ())
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

string
TdCollector::get_chat_title (int64_t chat_id) const
{
  auto it = __chat_titles.find (chat_id);
  if (it == __chat_titles.end ())
    {
      return "unknown chat";
    }
  return it->second;
}

void
extra_decoration_filters (TdArray<TdPtr<TdTxtEnt>> &deco_list,
			  const string &msgtxt)
{
  vector<char16_t> seq = get_c16_seq (msgtxt);
  size_t i = 0;
  while (i < seq.size ())
    {
      {
	const size_t last_offset = 14;
	if (i + last_offset < seq.size () && seq[i] == 0x0054
	    && seq[i + 1] == 0x0069 && seq[i + 2] == 0x0074
	    && seq[i + 3] == 0x006c && seq[i + 4] == 0x0065
	    && seq[i + 5] == 0x0073 && seq[i + 6] == 0x0020
	    && seq[i + 7] == 0x003c && seq[i + 8] == 0x0074
	    && seq[i + 9] == 0x0069 && seq[i + 10] == 0x0074
	    && seq[i + 11] == 0x006c && seq[i + 12] == 0x0065
	    && seq[i + 13] == 0x0073 && seq[i + 14] == 0x003e)
	  {
	    deco_list.emplace_back (
	      td_mkobj<TdTxtEnt> (i, last_offset + 1,
				  td_mkobj<TdTxtEntBold> ()));
	  }
      }

      {
	const size_t last_offset = 16;
	if (i + last_offset < seq.size () && seq[i] == 0x0053
	    && seq[i + 1] == 0x0065 && seq[i + 2] == 0x006e
	    && seq[i + 3] == 0x0064 && seq[i + 4] == 0x0065
	    && seq[i + 5] == 0x0072 && seq[i + 6] == 0x0073
	    && seq[i + 7] == 0x0020 && seq[i + 8] == 0x003c
	    && seq[i + 9] == 0x0073 && seq[i + 10] == 0x0065
	    && seq[i + 11] == 0x006e && seq[i + 12] == 0x0064
	    && seq[i + 13] == 0x0065 && seq[i + 14] == 0x0072
	    && seq[i + 15] == 0x0073 && seq[i + 16] == 0x003e)
	  {
	    deco_list.emplace_back (
	      td_mkobj<TdTxtEnt> (i, last_offset + 1,
				  td_mkobj<TdTxtEntBold> ()));
	  }
      }

      {
	const size_t last_offset = 18;
	if (i + last_offset < seq.size () && seq[i] == 0x004b
	    && seq[i + 1] == 0x0065 && seq[i + 2] == 0x0079
	    && seq[i + 3] == 0x0077 && seq[i + 4] == 0x006f
	    && seq[i + 5] == 0x0072 && seq[i + 6] == 0x0064
	    && seq[i + 7] == 0x0073 && seq[i + 8] == 0x0020
	    && seq[i + 9] == 0x003c && seq[i + 10] == 0x006b
	    && seq[i + 11] == 0x0065 && seq[i + 12] == 0x0079
	    && seq[i + 13] == 0x0077 && seq[i + 14] == 0x006f
	    && seq[i + 15] == 0x0072 && seq[i + 16] == 0x0064
	    && seq[i + 17] == 0x0073 && seq[i + last_offset] == 0x003e)
	  {
	    deco_list.emplace_back (
	      td_mkobj<TdTxtEnt> (i, last_offset + 1,
				  td_mkobj<TdTxtEntBold> ()));
	  }
      }

      {
	const size_t last_offset = 20;
	if (i + last_offset < seq.size () && seq[i] == 0x004e
	    && seq[i + 1] == 0x004f && seq[i + 2] == 0x0020
	    && seq[i + 3] == 0x0054 && seq[i + 4] == 0x0069
	    && seq[i + 5] == 0x0074 && seq[i + 6] == 0x006c
	    && seq[i + 7] == 0x0065 && seq[i + 8] == 0x0073
	    && seq[i + 9] == 0x0020 && seq[i + 10] == 0x003c
	    && seq[i + 11] == 0x006e && seq[i + 12] == 0x006f
	    && seq[i + 13] == 0x002d && seq[i + 14] == 0x0074
	    && seq[i + 15] == 0x0069 && seq[i + 16] == 0x0074
	    && seq[i + 17] == 0x006c && seq[i + 18] == 0x0065
	    && seq[i + 19] == 0x0073 && seq[i + 20] == 0x003e)
	  {
	    deco_list.emplace_back (
	      td_mkobj<TdTxtEnt> (i, last_offset + 1,
				  td_mkobj<TdTxtEntBold> ()));
	  }
      }

      {
	const size_t last_offset = 22;
	if (i + last_offset < seq.size () && seq[i] == 0x004e
	    && seq[i + 1] == 0x004f && seq[i + 2] == 0x0020
	    && seq[i + 3] == 0x0053 && seq[i + 4] == 0x0065
	    && seq[i + 5] == 0x006e && seq[i + 6] == 0x0064
	    && seq[i + 7] == 0x0065 && seq[i + 8] == 0x0072
	    && seq[i + 9] == 0x0073 && seq[i + 10] == 0x0020
	    && seq[i + 11] == 0x003c && seq[i + 12] == 0x006e
	    && seq[i + 13] == 0x006f && seq[i + 14] == 0x002d
	    && seq[i + 15] == 0x0073 && seq[i + 16] == 0x0065
	    && seq[i + 17] == 0x006e && seq[i + 18] == 0x0064
	    && seq[i + 19] == 0x0065 && seq[i + 20] == 0x0072
	    && seq[i + 21] == 0x0073 && seq[i + 22] == 0x003e)
	  {
	    deco_list.emplace_back (
	      td_mkobj<TdTxtEnt> (i, last_offset + 1,
				  td_mkobj<TdTxtEntBold> ()));
	  }
      }

      {
	const size_t last_offset = 24;
	if (i + last_offset < seq.size () && seq[i] == 0x004e
	    && seq[i + 1] == 0x004f && seq[i + 2] == 0x0020
	    && seq[i + 3] == 0x004b && seq[i + 4] == 0x0065
	    && seq[i + 5] == 0x0079 && seq[i + 6] == 0x0077
	    && seq[i + 7] == 0x006f && seq[i + 8] == 0x0072
	    && seq[i + 9] == 0x0064 && seq[i + 10] == 0x0073
	    && seq[i + 11] == 0x0020 && seq[i + 12] == 0x003c
	    && seq[i + 13] == 0x006e && seq[i + 14] == 0x006f
	    && seq[i + 15] == 0x002d && seq[i + 16] == 0x006b
	    && seq[i + 17] == 0x0065 && seq[i + 18] == 0x0079
	    && seq[i + 19] == 0x0077 && seq[i + 20] == 0x006f
	    && seq[i + 21] == 0x0072 && seq[i + 22] == 0x0064
	    && seq[i + 23] == 0x0073 && seq[i + 24] == 0x003e)
	  {
	    deco_list.emplace_back (
	      td_mkobj<TdTxtEnt> (i, last_offset + 1,
				  td_mkobj<TdTxtEntBold> ()));
	  }
      }

      i++;
    }
}

void
TdCollector::handle_tgfcmd (string &&incom_txt)
{
  string aux_msg = string (CHATCMD_RPLY_PREFIX) + "not supported";
  string did_what = incom_txt;
  u32 len_did_what = did_what.length ();
  TdArray<TdPtr<TdTxtEnt>> deco_list;
  TdPtr<TdFmtTxt> message_text
    = td_mkobj<TdFmtTxt> (did_what + aux_msg, move (deco_list));

  if (incom_txt.find (CHATCMD_PAUSE) != string::npos)
    {
      tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdPause, incom_txt,
			       tgfstat::userdata);
      // succ_data = move (res.succ_data ())
      aux_msg = move (res.aux_msg ());
      did_what = move (res.did_what ().value ());
      len_did_what = did_what.length ();
    }
  else if (incom_txt.find (CHATCMD_RESUME) != string::npos)
    {
      tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdResume, incom_txt,
			       tgfstat::userdata);
      // succ_data = move (res.succ_data ())
      aux_msg = move (res.aux_msg ());
      did_what = move (res.did_what ().value ());
      len_did_what = did_what.length ();
    }
  else if (incom_txt.find (CHATCMD_FILTERS) != string::npos)
    {
      tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdFilters, incom_txt,
			       tgfstat::userdata);
      // succ_data = move (res.succ_data ())
      aux_msg = move (res.aux_msg ());
      did_what = move (res.did_what ().value ());
      len_did_what = did_what.length ();
    }
  else if (incom_txt.find (CHATCMD_RAWFILTERS) != string::npos)
    {
      tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRawFilters, incom_txt,
			       tgfstat::userdata);
      // succ_data = move (res.succ_data ())
      aux_msg = move (res.aux_msg ());
      did_what = move (res.did_what ().value ());
      len_did_what = did_what.length ();
    }
  else if (incom_txt.find (CHATCMD_EDITF) != string::npos)
    {
      string curr_filters = tgfstat::userdata.get_filters ();
      tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, incom_txt,
			       tgfstat::userdata);
      aux_msg = move (res.aux_msg ());
      did_what = move (res.did_what ().value ());
      len_did_what = did_what.length ();
    }
  else if (incom_txt.find (CHATCMD_RMF) != string::npos)
    {
      string curr_filters = tgfstat::userdata.get_filters ();
      tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, incom_txt,
			       tgfstat::userdata);
      aux_msg = move (res.aux_msg ());
      did_what = move (res.did_what ().value ());
      len_did_what = did_what.length ();
    }

  // tulogfe_cg (111, len_did_what, (string ("") + did_what + aux_msg));
  size_t utf16len_didwhat = get_c16_len (did_what);
  message_text->text_ = did_what + aux_msg;
  // quote
  deco_list.emplace_back (
    td_mkobj<TdTxtEnt> (0, utf16len_didwhat, td_mkobj<TdTxtEntQuote> ()));
  // emphasize
  deco_list.emplace_back (td_mkobj<TdTxtEnt> (utf16len_didwhat,
					      CHATCMD_RPLY_PREFIX_N_CP,
					      td_mkobj<TdTxtEntBold> ()));

  if (incom_txt.find (CHATCMD_FILTERS) != string::npos)
    extra_decoration_filters (deco_list, message_text->text_);

  message_text->entities_ = move (deco_list);

  TdPtr<TdFunc> send_message_request = td_mkobj<TdSendMsg> (
    // this->collector_id, //
    tgfstat::userdata.get_tgfid (), //
    0, nullptr, nullptr, nullptr,
    td_mkobj<TdInMsgTxt> (move (message_text), nullptr, true));

  send_query (move (send_message_request), [this] (TdObjPtr object) {
    if (object->get_id () == TdMsg::ID)
      {
	// FIXME: do not use operator <<
	tulogfi_cg (1, "TGFCMD reply send");
      }
    else if (object->get_id () == TdErr::ID)
      {
	auto error = tl_movas<TdErr> (object);
	tulogfe_cg (1, " error code:", error->code_,
		    " error message:", error->message_);
      }
  });
}

void
TdCollector::process_update (TdObjPtr update)
{
  switch (update->get_id ())
    {
      case TdUpdAuthStat::ID: {
	auto casted = static_cast<TdUpdAuthStat *> (update.get ());
	this->__auth_stat = move (casted->authorization_state_);
	this->on_authorization_state_update ();
	break;
      }

      case TdUpdNewChat::ID: {
	auto casted = static_cast<TdUpdNewChat *> (update.get ());
	this->__chat_titles[casted->chat_->id_] = casted->chat_->title_;
	break;
      }

      case TdUpdChatTitle::ID: {
	auto casted = static_cast<TdUpdChatTitle *> (update.get ());
	this->__chat_titles[casted->chat_id_] = casted->title_;
	break;
      }

      case TdUpdUser::ID: {
	auto casted = static_cast<TdUpdUser *> (update.get ());
	auto user_id = casted->user_->id_;
	this->__users[user_id] = move (casted->user_);
	break;
      }

      case TdUpdNewMsg::ID: {
	TdUpdNewMsg *nmsg = static_cast<TdUpdNewMsg *> (update.get ());
	TdInt chat_id = nmsg->message_->chat_id_;
	string chat_title = __chat_titles[chat_id];

	if (chat_title.find ("TG-FOCUS") != string::npos)
	  {
	    TdPtr<TdMsgContent> p = move (nmsg->message_->content_);
	    if (p->get_id () == TdMsgText::ID)
	      {
		// NOT WORKED: no copy ctor, will error:
		//    TdMsgText r = static_cast<TdMsgText> (*p);
		// TODO: why, when using static cast,
		//       use aliased func not allowed?:
		//    TdMsgText &r = scast<TdMsgText &> (*p);
		//       meanwhile the original one worked:
		//    TdMsgText &r = static_cast<TdMsgText &> (*p);
		// TODO: why, dynamic_cast reports undefine ref for typeinfo
		TdMsgText &r = static_cast<TdMsgText &> (*p);
		string txt = move (r.text_->text_);
		txt.erase (0, txt.find_first_not_of (" ", 0));
		if (txt.length () > 6 && txt[6] == 0x20
		    && txt.find ("TGFCMD") == 0)
		  {
		    txt.replace (0, 6, "");
		    handle_tgfcmd (move (txt));
		  }
	      }

	    return;
	  }

	string sender_name;
	TdPtr<TdMsgSender> sender_id = move (nmsg->message_->sender_id_);
	switch (sender_id->get_id ())
	  {
	    case TdMsgSenderUser::ID: {
	      auto nmsg = static_cast<TdMsgSenderUser *> (sender_id.get ());
	      sender_name = get_user_name (nmsg->user_id_);
	      break;
	    }
	    case TdMsgSenderChat::ID: {
	      auto nmsg = static_cast<TdMsgSenderChat *> (sender_id.get ());
	      sender_name += get_chat_title (nmsg->chat_id_);
	      sender_name += "(chat)";
	      // sender_name
	      // "{}(chat)"
	      break;
	    }
	  }

	string text;
	switch (nmsg->message_->content_->get_id ())
	  {
	    case TdMsgText::ID: {
	      auto orig_txt_ctn
		= static_cast<TdMsgText &> (*nmsg->message_->content_)
		    .text_->text_;
	      text = move (orig_txt_ctn);
	      break;
	    }

	    case TdMsgPhoto::ID: {
	      auto orig_txt_ctn
		= static_cast<TdMsgPhoto &> (*nmsg->message_->content_)
		    .caption_->text_;

	      text += "<photo>";
	      text += "(";
	      text += move (orig_txt_ctn);
	      text += ")";
	      // "<photo>({})"
	      break;
	    }

	    case TdMsgAniEmoji::ID: {
	      auto orig_txt_ctn
		= static_cast<TdMsgAniEmoji &> (*nmsg->message_->content_)
		    .emoji_;
	      text += "<emoji>";
	      text += "(";
	      text += move (orig_txt_ctn);
	      text += ")";
	      // "<emoji>({})"
	      break;
	    }

	    case TdMsgSticker::ID: {
	      auto orig_txt_ctn
		= static_cast<TdMsgSticker &> (*nmsg->message_->content_)
		    .sticker_->emoji_;
	      text += "<sticker>";
	      text += "(";
	      text += move (orig_txt_ctn);
	      text += ")";
	      // "<sticker>({})"
	      break;
	    }

	    default: {
	      text += "ignored message with ID ";
	      text += nmsg->message_->content_->get_id ();
	      // "ignored message with ID {}"
	      break;
	    }
	  }

	int32_t tstamp = nmsg->message_->date_;

	// ---
	lock_guard<mutex> mq_guard (tgfstat::c::d::mq_lock);

	tgf::TgMsg msg (__chat_titles[chat_id], sender_name, text, tstamp);
	tgfstat::c::d::mq.insert (tgfstat::c::d::mq.begin (), move (msg));
	tgfstat::c::d::it_msg_total.fetch_add (1, mo::relaxed);

	break;
      }

      default: {
	tgf::logfd_cg (1, "producer_iter:",
		       tgfstat::c::d::it_cnt_producer.load (mo::relaxed),
		       " ignored update with id:", update->get_id ());
	break;
      }
    }
}

function<void (TdObjPtr)>
TdCollector::auth_query_callback ()
{
  return [this, id = __auth_qry_id] (TdObjPtr object) {
    if (id == __auth_qry_id)
      {
	// FIXME: logging
	if (object->get_id () == TdErr::ID)
	  {
	    auto error = tl_movas<TdErr> (object);
	    tgf::logfd_cg (1, 997755, to_string (error));
	  }
      }
  };
}

void
TdCollector::on_authorization_state_update ()
{
  this->__auth_qry_id++;

  switch (this->__auth_stat->get_id ())
    {
      case TdAuthStatReady::ID: {
	this->is_auth = true;
	tgf::logfd_cg (1, "Authorization is completed");
	break;
      }

      case TdAuthStatWaitPara::ID: {
	tgf::logfd_cg (1, "wait tdlib para... ");

	string inbuf;

	auto request = td_mkobj<TdSetPara> ();
	request->database_directory_ = tgfstat::userdata.path_tddata ();
	request->use_message_database_ = true;
	request->use_secret_chats_ = true;
	request->api_id_ = tgfstat::userdata.get_api_id ();
	request->api_hash_ = tgfstat::userdata.get_api_hash (); //
	request->system_language_code_ = "en";
	request->device_model_ = TF_DEV;
	request->application_version_ = TF_VER;
	send_query (move (request), auth_query_callback ());
	break;
      }

      default: {
	int32_t authState = this->__auth_stat->get_id ();
	if (authState == TdAuthStatWaitPhone::ID)
	  {
	    tulogfe_cg (
	      1,
	      "Logged in information is "
	      "corrupted. Run commands one by one: "
	      "`auth-reset`, `auth`, to resolve the issue. Please report the "
	      "issue if this message still exists.");
	    exit (2);
	  }
	else
	  {
	    tulogfe_cg (1, "ignored auth state with id ", authState);
	  }
	break;
      }
    }
}

void
TdCollector::check_authentication_error (TdObjPtr object)
{
  if (object->get_id () == TdErr::ID)
    {
      auto error = tl_movas<TdErr> (object);
      tulogfe_cg (1, to_string (error));
      // cout << "Error: " << to_string (error) << flush;
    }
  else
    {
      cerr << "check auth no error, id:" << object->get_id () << endl;
    }
}

uint64_t
TdCollector::next_query_id ()
{
  return ++__curr_qry_id;
}
