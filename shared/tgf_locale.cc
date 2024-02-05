#include <tuple>
#include <locale.h>
#include <string>
#include <vector>
#include <string.h>
#include <iostream>

#include "tgf_locale.hh"

namespace tgf {

enum Lang HOST_LANG = Lang::unknown;
enum Lang PREFER_LANG = Lang::unknown;

//
// Currently supported languages: en_HK, en_US, zh_CN, zh_HK
//
const char *STRNAME_unknown = "?-?";
const char *STRNAME_en_HK = "en-HK";
const char *STRNAME_en_US = "en-US";
const char *STRNAME_en_ZW = "en-ZW";
const char *STRNAME_zh_CN = "zh-CN";
const char *STRNAME_zh_HK = "zh-HK";

extern Lang
lang_from_cstr (const char *cstr)
{
  if (strlen (cstr) < 5)
    return Lang::unknown;

  if (strcmp (cstr, STRNAME_en_HK) == 0)
    return Lang::en_HK;
  if (strcmp (cstr, STRNAME_en_US) == 0)
    return Lang::en_US;
  if (strcmp (cstr, STRNAME_en_ZW) == 0)
    return Lang::en_ZW;
  if (strcmp (cstr, STRNAME_zh_CN) == 0)
    return Lang::zh_CN;
  if (strcmp (cstr, STRNAME_zh_HK) == 0)
    return Lang::zh_HK;

  return Lang::unknown;
}

const char *
lang_to_cstr (Lang l)
{
  switch (l)
    {
    case Lang::en_HK:
      return STRNAME_en_HK;
      break;
    case Lang::en_US:
      return STRNAME_en_US;
      break;
    case Lang::en_ZW:
      return STRNAME_en_ZW;
    case Lang::zh_CN:
      return STRNAME_zh_CN;
      break;
    case Lang::zh_HK:
      return STRNAME_zh_HK;
      break;
    default:
      return STRNAME_unknown;
      break;
    }
}

std::ostream &
operator<< (std::ostream &os, const Lang l)
{
  os << lang_to_cstr (l);
  return os;
}

bool
try_ensure_locale ()
{
  using namespace std;

  // FIXME: use constant instead
  vector<tuple<const char *, Lang>> lclist = {
    // {"C.UTF-8", unknown}, // we dont need this
    {STRNAME_en_HK, en_HK}, {STRNAME_en_US, en_US}, {STRNAME_en_ZW, en_ZW},
    {STRNAME_zh_CN, zh_CN}, {STRNAME_zh_HK, zh_HK},
  };

  vector<tuple<const char *, Lang>> supported = {};

  const char *trailing_enc = ".UTF-8";
  size_t n_trailing_enc = 6;

  char *tmpbuf = (char *) alloca (6 + 6);
  for (const tuple<const char *, Lang> &t : lclist)
    {
      const char *strname = get<0> (t);
      size_t nchar = strlen (strname);
      memcpy (tmpbuf, strname, nchar);
      if (nchar == 5 && tmpbuf[2] == '-')
	{
	  tmpbuf[2] = '_';
	  memcpy (tmpbuf + 5, (void *) trailing_enc, n_trailing_enc);
	  tmpbuf[11] = 0;
	}
      else if (nchar == 6 && tmpbuf[3] == '-')
	{
	  tmpbuf[3] = '_';
	  memcpy (tmpbuf + 6, (void *) trailing_enc, n_trailing_enc);
	}
      if (setlocale (LC_ALL, tmpbuf) != nullptr)
	{
	  Lang lc = get<1> (t);

	  if (lc == PREFER_LANG)
	    {
	      HOST_LANG = lc;
	      return true;
	    }
	  else
	    {
	      supported.push_back (t);
	    }
	}
    }

  if (supported.size () > 0)
    {
      setlocale (LC_ALL, get<0> (supported[0]));
      HOST_LANG = get<1> (supported[0]);
      return true;
    }

  return false;
}

} // namespace tgf
