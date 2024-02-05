#include "tgf_locale.hh"
#include "tgf_msg.hh"
#include "common.hh"

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

std::string
TgMsg::to_locale_string () const
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
	ret += "-1";
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
	ret += "-1";
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
	ret += "[ CHAT ] ";
	ret += this->title_;
	ret += "\n";
	ret += "[ SENDER ] ";
	ret += this->sender_;
	ret += "\n";
	ret += "[ CONTENT ] ";
	ret += this->txt_;
	ret += "\n";
	ret += "[ DATE ] ";
	ret += this->tstamp_;
	ret += "\n";
	ret += "[ ID ] ";
	ret += "-1";
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

bool
can_decor ()
{
  return tgf::try_ensure_locale ();
}

std::string
decor_pos_to_str (const std::vector<std::tuple<int, int>> &pos)
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
  if (cuseq[begi] == 0x5b && begi + 7 < cuseq.size ())
    if (cuseq[begi + 1] == ' ' && cuseq[begi + 2] == 'C'
	&& cuseq[begi + 3] == 'H' && cuseq[begi + 4] == 'A'
	&& cuseq[begi + 5] == 'T' && cuseq[begi + 6] == ' '
	&& cuseq[begi + 7] == ']')
      return std::make_optional<size_t> (7);

  return {};
}

std::optional<size_t>
get_endi_sender (std::vector<char16_t> &cuseq, size_t begi)
{
  if (cuseq[begi] == 0x5b && begi + 9 < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x53
	&& cuseq[begi + 3] == 0x45 && cuseq[begi + 4] == 0x4e
	&& cuseq[begi + 5] == 0x44 && cuseq[begi + 6] == 0x45
	&& cuseq[begi + 7] == 0x52 && cuseq[begi + 8] == 0x20
	&& cuseq[begi + 9] == 0x5d)
      return std::make_optional<size_t> (9);

  return {};
}

std::optional<size_t>
get_endi_content (std::vector<char16_t> &cuseq, size_t begi)
{
  if (cuseq[begi] == 0x5b && begi + 10 < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x43
	&& cuseq[begi + 3] == 0x4f && cuseq[begi + 4] == 0x4e
	&& cuseq[begi + 5] == 0x54 && cuseq[begi + 6] == 0x45
	&& cuseq[begi + 7] == 0x4e && cuseq[begi + 8] == 0x54
	&& cuseq[begi + 9] == 0x20 && cuseq[begi + 10] == 0x5d)
      return std::make_optional<size_t> (10);

  return {};
}

std::optional<size_t>
get_endi_date (std::vector<char16_t> &cuseq, size_t begi)
{
  if (cuseq[begi] == 0x5b && begi + 7 < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x44
	&& cuseq[begi + 3] == 0x41 && cuseq[begi + 4] == 0x54
	&& cuseq[begi + 5] == 0x45 && cuseq[begi + 6] == 0x20
	&& cuseq[begi + 7] == 0x5d)
      return std::make_optional<size_t> (7);

  return {};
}

std::optional<size_t>
get_endi_id (std::vector<char16_t> &cuseq, size_t begi)
{
  if (cuseq[begi] == 0x5b && begi + 5 < cuseq.size ())
    if (cuseq[begi + 1] == 0x20 && cuseq[begi + 2] == 0x49
	&& cuseq[begi + 3] == 0x44 && cuseq[begi + 4] == 0x20
	&& cuseq[begi + 5] == 0x5d)
      return std::make_optional<size_t> (5);

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
get_decor_pos (const std::string &str)
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
