
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

std::string
decor_pos_to_str (const std::vector<std::tuple<int, int>> &pos)
{
  std::string ret;
  for (auto ele : pos)
    {
      ret += fmt::format ("<{},{}>,", std::get<0> (ele), std::get<1> (ele));
    }
  return ret;
}

std::vector<std::tuple<int, int>>
get_decor_pos (const std::string &str)
{
  std::vector<char16_t> cuseq{};
  std::mbstate_t state{};
  char16_t c16;

  const char *ptr = &str[0], *end = &str[0] + str.size ();

  while (size_t rc = mbrtoc16 (&c16, ptr, end - ptr + 1, &state))
    {
      if (rc == (size_t) -3)
	{
	  std::cout << "mbrtoc16 -3" << std::endl;
	  cuseq.push_back (c16);
	}
      else if (rc == (size_t) -2)
	{
	  std::cout << "mbrtoc16 -2" << std::endl;
	  return std::vector<std::tuple<int, int>>{};
	}
      else if (rc == (size_t) -1)
	{
	  std::cout << "mbrtoc16 -1" << std::endl;
	  return std::vector<std::tuple<int, int>>{};
	}
      else
	{
	  cuseq.push_back (c16);
	  ptr += rc;
	}
    }

  std::vector<std::tuple<int, int>> ret;
  for (size_t i = 0; i < cuseq.size (); i++)
    {
      if (cuseq[i] == 0x5b && i + 7 < cuseq.size ())
	if (cuseq[i + 1] == 0x20 && cuseq[i + 2] == 0x43 && cuseq[i + 3] == 0x48
	    && cuseq[i + 4] == 0x41 && cuseq[i + 5] == 0x54
	    && cuseq[i + 6] == 0x20 && cuseq[i + 7] == 0x5d)
	  {
	    ret.emplace_back (std::make_tuple (i, 7 + 1));
	  }

      if (cuseq[i] == 0x5b && i + 9 < cuseq.size ())
	if (cuseq[i + 1] == 0x20 && cuseq[i + 2] == 0x53 && cuseq[i + 3] == 0x45
	    && cuseq[i + 4] == 0x4e && cuseq[i + 5] == 0x44
	    && cuseq[i + 6] == 0x45 && cuseq[i + 7] == 0x52
	    && cuseq[i + 8] == 0x20 && cuseq[i + 9] == 0x5d)
	  {
	    ret.emplace_back (std::make_tuple (i, 9 + 1));
	  }

      if (cuseq[i] == 0x5b && i + 10 < cuseq.size ())
	if (cuseq[i + 1] == 0x20 && cuseq[i + 2] == 0x43 && cuseq[i + 3] == 0x4f
	    && cuseq[i + 4] == 0x4e && cuseq[i + 5] == 0x54
	    && cuseq[i + 6] == 0x45 && cuseq[i + 7] == 0x4e
	    && cuseq[i + 8] == 0x54 && cuseq[i + 9] == 0x20
	    && cuseq[i + 10] == 0x5d)
	  {
	    ret.emplace_back (std::make_tuple (i, 10 + 1));
	  }

      if (cuseq[i] == 0x5b && i + 7 < cuseq.size ())
	if (cuseq[i + 1] == 0x20 && cuseq[i + 2] == 0x44 && cuseq[i + 3] == 0x41
	    && cuseq[i + 4] == 0x54 && cuseq[i + 5] == 0x45
	    && cuseq[i + 6] == 0x20 && cuseq[i + 7] == 0x5d)
	  {
	    ret.emplace_back (std::make_tuple (i, 7 + 1));
	  }

      if (cuseq[i] == 0x5b && i + 5 < cuseq.size ())
	if (cuseq[i + 1] == 0x20 && cuseq[i + 2] == 0x49 && cuseq[i + 3] == 0x44
	    && cuseq[i + 4] == 0x20 && cuseq[i + 5] == 0x5d)
	  {
	    ret.emplace_back (std::make_tuple (i, 5 + 1));
	  }
    }

  return ret;
}
