#define __TU__ "posix_regex"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <optional>
#include <regex.h>

#include "std_comp.hh"
#include "posix_regex.hh"

PosixExtRegex::PosixExtRegex () : __stat_comp (INT_MAX), __re (nullptr) {}

PosixExtRegex::PosixExtRegex (PosixExtRegex &&move_ctor_from)
  : __stat_comp (move_ctor_from.__stat_comp), __re (move_ctor_from.__re), __ptn (move (move_ctor_from.__ptn))
{
  move_ctor_from.__stat_comp = INT_MAX;
  move_ctor_from.__re = nullptr;
}

PosixExtRegex::PosixExtRegex (const std::string &ptn)
{
  const char *ptn_ = ptn.c_str ();
  this->__re = reinterpret_cast<regex_t *> (malloc (sizeof (regex_t)));
  this->__stat_comp = regcomp (this->__re, ptn_, REG_EXTENDED | REG_NOSUB);
  __ptn = move (ptn);
}

PosixExtRegex &
PosixExtRegex::operator= (PosixExtRegex &&move_assign_from)
{
  this->__stat_comp = move_assign_from.__stat_comp;
  this->__re = move_assign_from.__re;
  __ptn = move (move_assign_from.__ptn);
  move_assign_from.__stat_comp = INT_MAX;
  move_assign_from.__re = nullptr;
  return *this;
}

PosixExtRegex::~PosixExtRegex ()
{
  if (this->__re != nullptr)
    {
      regfree (this->__re);
      free (this->__re); // not necessary!
    }
}

std::string
PosixExtRegex::get_err () noexcept
{
  if (this->__stat_comp != 0)
    {
      static constexpr size_t BUF_SIZE = 1024;
      char posix_re_errbuf[BUF_SIZE];
      const size_t nchar = regerror (this->__stat_comp, nullptr, posix_re_errbuf, BUF_SIZE);

      return std::string (posix_re_errbuf, nchar);
    }

  return std::string ();
}

std::optional<bool>
PosixExtRegex::is_match (const std::string &str) const noexcept
{
  if (this->__stat_comp != 0)
    return std::nullopt;

  auto str_ = str.c_str ();

  int stat_exec = regexec (this->__re, str_, (size_t) 0, nullptr, 0);

  if (stat_exec == REG_NOMATCH)
    {
      return false;
    }

  return true;
}
