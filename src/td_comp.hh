#ifndef hh_td_comp
#define hh_td_comp

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

namespace tapi = td::td_api;

using TdObjPtr = tapi::object_ptr<tapi::Object>;
template <typename O> using TdUniPtr = tapi::object_ptr<O>;
using TdFunc = tapi::Function;
using TdInt = tapi::int53;
using TdAuthStat = tapi::AuthorizationState;
using TdClient = td::ClientManager;
using TdErr = tapi::error;
using TdChat = tapi::chat;
using TdGetOpt = tapi::getOption;
using TdSetOpt = tapi::setOption;
using TdOptValBool = tapi::optionValueBoolean;
using TdSetLogLevel = tapi::setLogVerbosityLevel;
using TdCreatSuperChat = tapi::createNewSupergroupChat;
using TdCreatBasicChat = tapi::createNewBasicGroupChat;
using TdBasicChat = tapi::createdBasicGroupChat;
using TdTxtEnt = tapi::textEntity;
using TdTxtEntBold = tapi::textEntityTypeBold;
using TdTxtEntQuote = tapi::textEntityTypeBlockQuote;
using TdUpdAuthStat = tapi::updateAuthorizationState;
using TdUpdChatTitle = tapi::updateChatTitle;
using TdUpdNewChat = tapi::updateNewChat;
using TdUpdNewMsg = tapi::updateNewMessage;
using TdUpdUser = tapi::updateUser;
using TdMsgSenderUser = tapi::messageSenderUser;
using TdMsgSenderChat = tapi::messageSenderChat;
using TdMsgText = tapi::messageText;
using TdMsgPhoto = tapi::messagePhoto;
using TdMsgAniEmoji = tapi::messageAnimatedEmoji;
using TdMsgSticker = tapi::messageSticker;
using TdAuthStatReady = tapi::authorizationStateReady;
using TdAuthStatWaitPara = tapi::authorizationStateWaitTdlibParameters;
using TdAuthStatWaitPhone = tapi::authorizationStateWaitPhoneNumber;
using TdSetPara = tapi::setTdlibParameters;
using TdSendMsg = tapi::sendMessage;
using TdFmtTxt = tapi::formattedText;
using TdInMsgTxt = tapi::inputMessageText;
using TdMsg = tapi::message;
template <typename E> using TdArray = tapi::array<E>;
using TdUser = tapi::user;
using TdMsgSender = tapi::MessageSender;
using TdMsgContent = tapi::MessageContent;

template <class Type, class... Args>
td::tl_object_ptr<Type>
td_mkobj (Args &&...args)
{
  return td::td_api::make_object<Type> (std::forward<Args> (args)...);

  // template <class Type, class... Args>
  // object_ptr<Type> make_object(Args &&... args) {
  //   return object_ptr<Type>(new Type(std::forward<Args>(args)...));
  // }
}

template <class ToT, class FromT>
td::tl_object_ptr<ToT>
tl_movas (td::tl_object_ptr<FromT> &from)
{
  return td::move_tl_object_as<ToT> (from);

  // template <class ToT, class FromT>
  // tl_object_ptr<ToT> move_tl_object_as(tl_object_ptr<FromT> &from) {
  //   return tl_object_ptr<ToT>(static_cast<ToT *>(from.release()));
  // }
}

#endif
