#include "utfutils.hh"
#include <uchar.h>

size_t
get_c16_len (const string &utf8s)
{
  if (utf8s.length () == 0)
    return 0;
  size_t ret = 0;
  const char *const buf_utf8 = utf8s.data ();
  size_t n_cu_utf8 = utf8s.length (); // excluding NUL
  const char *bufp_utf8 = buf_utf8;
  const char *bufendp_utf8 = buf_utf8 + n_cu_utf8;
  if (n_cu_utf8 > 1024)
    {
      // support lands in the future
      return 0;
    }
  char16_t buf_utf16[n_cu_utf8];
  char16_t *bufp_utf16 = buf_utf16;
  size_t n_cu_utf16 = 0;
  size_t n_by_converted = -1;
  mbstate_t state = {0};
  while (n_by_converted != 0)
    {
      n_by_converted
	= mbrtoc16 (bufp_utf16, bufp_utf8, bufendp_utf8 - bufp_utf8, &state);
      if (n_by_converted == -1)
	{
	  break;
	}
      else if (n_by_converted == -2)
	{
	  break;
	}
      else if (n_by_converted == -3)
	{
	  bufp_utf16 += 1;
	}
      else
	{
	  bufp_utf8 += n_by_converted;
	  bufp_utf16 += 1;
	}
    }
  // NOTE that the extra one populated by libc is 0000, and placed at the end,
  // , actually should be a feff or fffe at the begining
  // return bufp_utf16 - buf_utf16 + 1;
  return bufp_utf16 - buf_utf16;
}

vector<char16_t>
get_c16_seq (const string &utf8s)
{
  if (utf8s.length () == 0)
    return {};
  size_t ret = 0;
  const char *const buf_utf8 = utf8s.data ();
  size_t n_cu_utf8 = utf8s.length (); // excluding NUL
  const char *bufp_utf8 = buf_utf8;
  const char *bufendp_utf8 = buf_utf8 + n_cu_utf8;
  if (n_cu_utf8 > 1024)
    {
      // support lands in the future
      return {};
    }
  char16_t buf_utf16[n_cu_utf8];
  char16_t *bufp_utf16 = buf_utf16;
  size_t n_by_converted = -1;
  mbstate_t state = {0};
  while (n_by_converted != 0)
    {
      n_by_converted
	= mbrtoc16 (bufp_utf16, bufp_utf8, bufendp_utf8 - bufp_utf8, &state);
      if (n_by_converted == -1)
	{
	  break;
	}
      else if (n_by_converted == -2)
	{
	  break;
	}
      else if (n_by_converted == -3)
	{
	  bufp_utf16 += 1;
	}
      else
	{
	  bufp_utf8 += n_by_converted;
	  bufp_utf16 += 1;
	}
    }

  size_t n_cu_utf16 = bufp_utf16 - buf_utf16;

  return vector (buf_utf16, buf_utf16 + n_cu_utf16);
}
