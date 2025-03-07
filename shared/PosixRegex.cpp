#include <cassert>
#include <cstdint>
#include <iostream>
#include <optional>
#include <regex.h>

#include "PosixRegex.hpp"

PosixExtRegex::PosixExtRegex () : stat_comp (INT_MAX), re (nullptr) {}

PosixExtRegex::PosixExtRegex (PosixExtRegex &&move_ctor_from)
  : stat_comp (move_ctor_from.stat_comp), re (move_ctor_from.re)
{
  move_ctor_from.stat_comp = INT_MAX;
  move_ctor_from.re = nullptr;
}

PosixExtRegex::PosixExtRegex (const std::string &ptn)
{
  auto ptn_ = ptn.c_str ();

  this->re = reinterpret_cast<regex_t *> (malloc (sizeof (regex_t)));
  this->stat_comp = regcomp (this->re, ptn_, REG_EXTENDED | REG_NOSUB);
}

PosixExtRegex &
PosixExtRegex::operator= (PosixExtRegex &&move_assign_from)
{
  this->stat_comp = move_assign_from.stat_comp;
  this->re = move_assign_from.re;
  move_assign_from.stat_comp = INT_MAX;
  move_assign_from.re = nullptr;
  return *this;
}

PosixExtRegex::~PosixExtRegex ()
{
  if (this->re != nullptr)
    {
      regfree (this->re);
      free (this->re); // not necessary!
    }
}

std::string
PosixExtRegex::get_err () noexcept
{
  if (this->stat_comp != 0)
    {
      static constexpr size_t BUF_SIZE = 1024;
      char posix_re_errbuf[BUF_SIZE];
      const size_t nchar
	= regerror (this->stat_comp, nullptr, posix_re_errbuf, BUF_SIZE);

      return std::string (posix_re_errbuf, nchar);
    }

  return std::string ();
}

std::optional<bool>
PosixExtRegex::is_match (const std::string &str) noexcept
{
  if (this->stat_comp != 0)
    return std::nullopt;

  auto str_ = str.c_str ();

  int stat_exec = regexec (this->re, str_, (size_t) 0, nullptr, 0);

  if (stat_exec == REG_NOMATCH)
    {
      return false;
    }

  return true;
}
