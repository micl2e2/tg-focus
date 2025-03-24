#ifndef hh_tg_msg
#define hh_tg_msg

#include "std_comp.hh"
#include <iostream>
#include <cstdint>
#include <string>
#include <optional>
#include <time.h>
#include <tuple>
#include <uchar.h>
#include <vector>

// uncomment this to enable deps hell, which make this type disappeared somehow
// #include "common.hh"

namespace tgf {

class TgMsg
{
public:
  TgMsg () = default;

  TgMsg (std::string &chat_title, std::string &sender,
	 std::string &text_content, int32_t tstamp = 0);

  TgMsg (std::string &&chat_title, std::string &&sender,
	 std::string &&text_content, int32_t tstamp = 0);

  inline const std::string &get_chat_title () const { return this->title_; }

  inline bool is_from_tgfocus () const
  {
    return this->title_.find ("TG-FOCUS") != string::npos;
  }

  inline const std::string &get_sender () const { return this->sender_; }

  inline const std::string &get_text_content () const { return this->txt_; }

  inline const std::string &get_timestamp () const { return this->tstamp_; }

  inline void set_id (uint32_t n) { this->id_ = n; }

  std::string to_locale_string () const;

  std::string to_string () const
  {
    std::string ret = "";

    ret += "Telegram Message";
    ret += "-<title,";
    ret += this->title_;
    ret += ">";
    ret += "-<sender,";
    ret += this->sender_;
    ret += ">";
    ret += "-<txt,";
    ret += this->txt_;
    ret += ">";
    ret += "-<tstamp,";
    ret += this->tstamp_;
    ret += ">";

    return ret;
  }

  friend std::ostream &operator<< (std::ostream &os, const TgMsg &msg);

private:
  std::string title_;
  std::string sender_;
  std::string txt_;
  std::string tstamp_;
  std::uint32_t id_;
};

// Check host environment supports decoration.
// Note: If it is decorable, this will change the default locale.
bool
can_decor ();

//
// Decorate the incoming TfMsg if any suitable locale is supported on host,
// otherwise rendering a empty result.
//
std::vector<std::tuple<int, int>>
get_decor_pos (const std::string &str);

std::string
decor_pos_to_str (const std::vector<std::tuple<int, int>> &pos);

} // namespace tgf

#endif
