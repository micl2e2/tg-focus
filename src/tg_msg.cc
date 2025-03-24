#define __TU__ "tg_msg"

#include "tg_msg.hh"
#include "locale_conf.hh"

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

tgf::TgMsg::TgMsg (std::string &&chat_title, std::string &&sender,
	      std::string &&text_content, int32_t tstamp)
{
  this->title_ = std::move (chat_title);
  this->sender_ = std::move (sender);
  this->txt_ = std::move (text_content);
  this->tstamp_ = make_timestamp_readable (tstamp);
  this->id_ = 0;
}

tgf::TgMsg::TgMsg (std::string &chat_title, std::string &sender,
	      std::string &text_content, int32_t tstamp)
{
  this->title_ = (chat_title);
  this->sender_ = (sender);
  this->txt_ = (text_content);
  this->tstamp_ = make_timestamp_readable (tstamp);
  this->id_ = 0;
}

std::string
tgf::TgMsg::to_locale_string () const
{
  std::string ret;

  switch (tgf::HOST_LANG)
    {
      //
      case tgf::Lang::zh_HK: {
	ret += "[ 群組 ] ";
	ret += this->title_;
	ret += "\n";
	ret += "[ 用戶 ] ";
	ret += this->sender_;
	ret += "\n";
	ret += "[ 訊息 ] ";
	ret += this->txt_;
	ret += "\n";
	ret += "[ 時間 ] ";
	ret += this->tstamp_;
	ret += "\n";
	ret += "[ 標識 ] ";
	ret += std::to_string (this->id_);
	ret += "\n";

	// R"([ 群组 ] {}
	// [ 用户 ] {}
	// [ 信息 ] {}
	// [ 时间 ] {}
	// [ 标识 ] {}
	// )"
	break;
      }
      //
      case tgf::Lang::zh_CN: {
	ret += "[ 群组 ] ";
	ret += this->title_;
	ret += "\n";
	ret += "[ 用户 ] ";
	ret += this->sender_;
	ret += "\n";
	ret += "[ 信息 ] ";
	ret += this->txt_;
	ret += "\n";
	ret += "[ 时间 ] ";
	ret += this->tstamp_;
	ret += "\n";
	ret += "[ 标识 ] ";
	ret += std::to_string (this->id_);
	ret += "\n";

	// R"([ 群组 ] {}
	// [ 用户 ] {}
	// [ 信息 ] {}
	// [ 时间 ] {}
	// [ 标识 ] {}
	// )"
	break;
      }

      default: {
	ret += "\xf0\x9f\x92\xa1 | ";
	ret += this->title_;
	ret += "\n";
	ret += "\xe2\x9c\x89 | ";
	ret += this->sender_;
	ret += "\n";
	ret += "\xf0\x9f\x92\xac | "; // "\u1f4ac" is only for <=FFFF
				      // \U... is compile-time error
	ret += this->txt_;
	ret += "\n";
	ret += "\xf0\x9f\x95\x94 | ";
	ret += this->tstamp_;
	ret += "\n";
	ret += "\xe2\x99\xbe | ";
	ret += std::to_string (this->id_);
	ret += "\n";

	// R"([ CHAT ] {}
	// [ SENDER ] {}
	// [ CONTENT ] {}
	// [ DATE ] {}
	// [ ID ] {}
	// )"
	break;
      }
    }

  return (ret);
}

namespace tgf{
  
std::ostream &
operator<< (std::ostream &os, const tgf::TgMsg &msg)
{
  os << "Telegram Message-"
     << "<title," << msg.title_ << ">"
     << "<sender," << msg.sender_ << ">"
     << "<txt," << msg.txt_ << ">"
     << "<tstamp," << msg.tstamp_ << ">"
     << "<id," << msg.id_ << ">";
  return os;
}

}

bool
tgf::can_decor ()
{
  return tgf::try_ensure_locale ();
}

std::string
tgf::decor_pos_to_str (const std::vector<std::tuple<int, int>> &pos)
{
  std::string ret;
  for (auto ele : pos)
    {
      ret += "<";
      ret += std::get<0> (ele);
      ret += ",";
      ret += std::get<1> (ele);
      ret += ">";
      ret += ",";
      // "<{},{}>,"
    }
  return ret;
}

namespace l_en_us {
std::optional<size_t>
get_endi_chat (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr int MAX_STEP = 4;
  if (cuseq[begi] == 0xd83d /* little endian */
      && begi + MAX_STEP < cuseq.size ())
    if (cuseq[begi + 1] == 0xdca1 && cuseq[begi + 2] == 0x20
	&& cuseq[begi + 3] == 0x5b && cuseq[begi + MAX_STEP] == 0x20)
      return std::make_optional<size_t> (MAX_STEP);

  return {};
}

std::optional<size_t>
get_endi_sender (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr int MAX_STEP = 3;
  if (cuseq[begi] == 0x7d44 && begi + MAX_STEP < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x5b
	&& cuseq[begi + MAX_STEP] == 0x20)
      return std::make_optional<size_t> (MAX_STEP);

  return {};
}

std::optional<size_t>
get_endi_content (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr int MAX_STEP = 4;
  if (cuseq[begi] == 0xd83d /* little endian */
      && begi + MAX_STEP < cuseq.size ())
    if (cuseq[begi + 1] == 0xdcac && cuseq[begi + 2] == 0x20
	&& cuseq[begi + 3] == 0x5b && cuseq[begi + MAX_STEP] == 0x20)
      return std::make_optional<size_t> (MAX_STEP);

  return {};
}

std::optional<size_t>
get_endi_date (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr int MAX_STEP = 4;
  if (cuseq[begi] == 0xd83d /* little endian */
      && begi + MAX_STEP < cuseq.size ())
    if (cuseq[begi + 1] == 0xdd54 && cuseq[begi + 2] == 0x20
	&& cuseq[begi + 3] == 0x5b && cuseq[begi + MAX_STEP] == 0x20)
      return std::make_optional<size_t> (MAX_STEP);

  return {};
}

std::optional<size_t>
get_endi_id (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr int MAX_STEP = 3;
  if (cuseq[begi] == 0x267e && begi + MAX_STEP < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x5b
	&& cuseq[begi + MAX_STEP] == 0x20)
      return std::make_optional<size_t> (MAX_STEP);

  return {};
}
} // namespace l_en_us

namespace l_zh_hk { // --------------------------------------------------------

std::optional<size_t>
get_endi_chat (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x7fa4
	&& cuseq[begi + 3] == 0x7d44 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}

std::optional<size_t>
get_endi_sender (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x7528
	&& cuseq[begi + 3] == 0x6236 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}

std::optional<size_t>
get_endi_content (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x8a0a
	&& cuseq[begi + 3] == 0x606f && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}

std::optional<size_t>
get_endi_date (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x6642
	&& cuseq[begi + 3] == 0x9593 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}

std::optional<size_t>
get_endi_id (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x6a19
	&& cuseq[begi + 3] == 0x8b58 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}
} // namespace l_zh_hk

namespace l_zh_cn {

std::optional<size_t>
get_endi_chat (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x7fa4
	&& cuseq[begi + 3] == 0x7ec4 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}

std::optional<size_t>
get_endi_sender (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x7528
	&& cuseq[begi + 3] == 0x6237 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}

std::optional<size_t>
get_endi_content (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x4fe1
	&& cuseq[begi + 3] == 0x606f && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}

std::optional<size_t>
get_endi_date (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x65f6
	&& cuseq[begi + 3] == 0x95f4 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}

std::optional<size_t>
get_endi_id (std::vector<char16_t> &cuseq, size_t begi)
{
  constexpr size_t n = 5;
  if (cuseq[begi] == 0x5b && begi + n < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x6807
	&& cuseq[begi + 3] == 0x8bc6 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + n] == 0x5d)
      return std::make_optional<size_t> (n);

  return {};
}
} // namespace l_zh_cn

std::optional<size_t>
get_endi_chat (std::vector<char16_t> &cuseq, size_t begi)
{
  switch (tgf::HOST_LANG)
    {
    case tgf::Lang::en_AU:
    case tgf::Lang::en_HK:
    case tgf::Lang::en_GB:
    case tgf::Lang::en_US:
      return l_en_us::get_endi_chat (cuseq, begi);
    case tgf::Lang::zh_HK:
      return l_zh_hk::get_endi_chat (cuseq, begi);
    case tgf::Lang::zh_CN:
      return l_zh_cn::get_endi_chat (cuseq, begi);
    default:
      return {};
    }
}

std::optional<size_t>
get_endi_sender (std::vector<char16_t> &cuseq, size_t begi)
{
  switch (tgf::HOST_LANG)
    {
    case tgf::Lang::en_AU:
    case tgf::Lang::en_HK:
    case tgf::Lang::en_GB:
    case tgf::Lang::en_US:
      return l_en_us::get_endi_sender (cuseq, begi);
    case tgf::Lang::zh_HK:
      return l_zh_hk::get_endi_sender (cuseq, begi);
    case tgf::Lang::zh_CN:
      return l_zh_cn::get_endi_sender (cuseq, begi);
    default:
      return {};
    }
}

std::optional<size_t>
get_endi_content (std::vector<char16_t> &cuseq, size_t begi)
{
  switch (tgf::HOST_LANG)
    {
    case tgf::Lang::en_AU:
    case tgf::Lang::en_HK:
    case tgf::Lang::en_GB:
    case tgf::Lang::en_US:
      return l_en_us::get_endi_content (cuseq, begi);
    case tgf::Lang::zh_HK:
      return l_zh_hk::get_endi_content (cuseq, begi);
    case tgf::Lang::zh_CN:
      return l_zh_cn::get_endi_content (cuseq, begi);
    default:
      return {};
    }
}

std::optional<size_t>
get_endi_date (std::vector<char16_t> &cuseq, size_t begi)
{
  switch (tgf::HOST_LANG)
    {
    case tgf::Lang::en_AU:
    case tgf::Lang::en_HK:
    case tgf::Lang::en_GB:
    case tgf::Lang::en_US:
      return l_en_us::get_endi_date (cuseq, begi);
    case tgf::Lang::zh_HK:
      return l_zh_hk::get_endi_date (cuseq, begi);
    case tgf::Lang::zh_CN:
      return l_zh_cn::get_endi_date (cuseq, begi);
    default:
      return {};
    }
}

std::optional<size_t>
get_endi_id (std::vector<char16_t> &cuseq, size_t begi)
{
  switch (tgf::HOST_LANG)
    {
    case tgf::Lang::en_AU:
    case tgf::Lang::en_HK:
    case tgf::Lang::en_GB:
    case tgf::Lang::en_US:
      return l_en_us::get_endi_id (cuseq, begi);
    case tgf::Lang::zh_HK:
      return l_zh_hk::get_endi_id (cuseq, begi);
    case tgf::Lang::zh_CN:
      return l_zh_cn::get_endi_id (cuseq, begi);
    default:
      return {};
    }
}

std::vector<std::tuple<int, int>>
tgf::get_decor_pos (const std::string &str)
{
  if (tgf::HOST_LANG == tgf::Lang::unknown)
    return std::vector<std::tuple<int, int>>{};

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
      if (auto endi = get_endi_chat (cuseq, i))
	{
	  ret.emplace_back (std::make_tuple (i, *endi + 1));
	}
      if (auto endi = get_endi_sender (cuseq, i))
	{
	  ret.emplace_back (std::make_tuple (i, *endi + 1));
	}
      if (auto endi = get_endi_content (cuseq, i))
	{
	  ret.emplace_back (std::make_tuple (i, *endi + 1));
	}
      if (auto endi = get_endi_date (cuseq, i))
	{
	  ret.emplace_back (std::make_tuple (i, *endi + 1));
	}
      if (auto endi = get_endi_id (cuseq, i))
	{
	  ret.emplace_back (std::make_tuple (i, *endi + 1));
	}
    }

  return ret;
}
