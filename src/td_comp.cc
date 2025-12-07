#include "td_comp.hh"

template<>
std::string
tapi_stringify (tapi::formattedText &r)
{
  std::string ret;
  ret += "(formattedText,";

  ret += "s:";
  ret += (r.text_);
  ret += ",";

  ret += ")";
  return ret;
}

template<>
std::string
tapi_stringify (tapi::message &r)
{
  std::string ret;
  ret += "(message,";

  ret += "id:";
  ret += std::to_string (r.id_);
  ret += ",";

  ret += "chat_id:";
  ret += std::to_string (r.chat_id_);
  ret += ",";

  ret += "content:";
  // ret += stringify_MessageContent (static_cast<tapi::MessageContent &> (*r.content_));
  ret += ",";

  ret += ")";
  return ret;
}
