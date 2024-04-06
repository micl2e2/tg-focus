#ifndef _COMMON_H
#define _COMMON_H 1

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

constexpr auto TF_DEV = "TG-FOCUS";
constexpr auto TF_VER = "1.7";
constexpr auto TF_COLL_CHAT_TITLE = "TG-FOCUS";
constexpr auto TDLIB_VER = "1.8.26";
constexpr auto TDLIB_SRC = "cd25b29452644856b7472dd0cd69391b2226d3c3";

class FileReader
{
public:
  bool has_error{false};

  FileReader () = delete;

  FileReader (const char *filename)
  {
    auto open_mode = "r";
    auto filestream = fopen (filename, open_mode);
    if (filestream != nullptr)
      {
	fseek (filestream, 0L, SEEK_END);
	size_t fsize = ftell (filestream);

	if (fsize > this->MAX_SMALL)
	  this->large_rbuf
	    = reinterpret_cast<char *> (malloc (this->MAX_LARGE));

	this->fstrm_ = filestream;
      }
    else
      {
	this->has_error = true;
      }
  }

  ~FileReader ()
  {
    if (this->fstrm_ != nullptr)
      fclose (this->fstrm_);
    if (this->large_rbuf != nullptr)
      free (this->large_rbuf);
  }

  std::optional<std::string> read_to_string ()
  {
    if (this->large_rbuf == nullptr)
      return this->read_small_to_string ();
    else
      return this->read_large_to_string ();
  }

private:
  static constexpr size_t MAX_SMALL{4096};
  static constexpr size_t MAX_LARGE{1 << 20};
  FILE *fstrm_{nullptr};
  char *large_rbuf{nullptr};

  std::optional<std::string> read_small_to_string ()
  {
    fseek (this->fstrm_, 0L, SEEK_SET);

    char rbuf[this->MAX_SMALL];
    auto nread = fread (rbuf, 1, this->MAX_SMALL, this->fstrm_);

    if (ferror (this->fstrm_) != 0 || feof (this->fstrm_) == 0)
      return std::nullopt;

    return std::string{rbuf, nread};
  }

  std::optional<std::string> read_large_to_string ()
  {
    fseek (this->fstrm_, 0L, SEEK_SET);
    auto nread = fread (this->large_rbuf, 1, this->MAX_LARGE, this->fstrm_);

    if (ferror (this->fstrm_) != 0 || feof (this->fstrm_) == 0)
      return std::nullopt;

    return std::string{this->large_rbuf, nread};
  }
};

// trim from start (in place)
static inline void
ltrim (std::string &s)
{
  s.erase (s.begin (),
	   std::find_if (s.begin (), s.end (),
			 [] (unsigned char ch) { return !std::isspace (ch); }));
}

// trim from end (in place)
static inline void
rtrim (std::string &s)
{
  s.erase (std::find_if (s.rbegin (), s.rend (),
			 [] (unsigned char ch) { return !std::isspace (ch); })
	     .base (),
	   s.end ());
}

// trim from both ends (in place)
static inline void
trim (std::string &s)
{
  rtrim (s);
  ltrim (s);
}

inline bool
is_valid_int32 (const std::string &in)
{
  long as_num = std::strtol (in.c_str (), nullptr, 10);

  if (as_num < INT_MIN || as_num > INT_MAX)
    return false;

  if (std::to_string (as_num) != in)
    return false;

  return true;
}

inline int32_t
sstr_to_int32 (const std::string &in)
{
  return static_cast<int32_t> (strtol (in.c_str (), nullptr, 10));
}

#endif
