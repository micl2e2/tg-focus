#include <stdint.h>
#include <time.h>
#include <iostream>

#include "tf_msg.hh"

std::string
make_timestamp_readable (uint32_t tstamp)
{
  time_t tt = static_cast<time_t> (tstamp);
  struct tm *bd_time = localtime (&tt);
  char buf[64];
  size_t nbytes = strftime (buf, sizeof (buf), "%Y-%m-%d %T %z %Z", bd_time);
  if (nbytes > 0)
    return std::string (buf, nbytes);
  return std::string ("unknown timestamp");
}

TgMsg::TgMsg (std::string &&chat_title, std::string &&sender,
	      std::string &&text_content, int32_t tstamp)
{
  this->title_ = std::move (chat_title);
  this->sender_ = std::move (sender);
  this->txt_ = std::move (text_content);
  this->tstamp_ = make_timestamp_readable (tstamp);
}

TgMsg::TgMsg (std::string &chat_title, std::string &sender,
	      std::string &text_content, int32_t tstamp)
{
  this->title_ = (chat_title);
  this->sender_ = (sender);
  this->txt_ = (text_content);
  this->tstamp_ = make_timestamp_readable (tstamp);
}

std::ostream &
operator<< (std::ostream &os, const TgMsg &msg)
{
  os << "Telegram Message-"
     << "<title," << msg.title_ << ">"
     << "<sender," << msg.sender_ << ">"
     << "<txt," << msg.txt_ << ">"
     << "<tstamp," << msg.tstamp_ << ">";
  return os;
}
