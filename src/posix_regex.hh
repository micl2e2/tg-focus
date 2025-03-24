#ifndef hh_posix_regex
#define hh_posix_regex

#include <assert.h>
#include <stdint.h>
#include <limits.h>
#include <regex.h>
#include <stddef.h>
#include <stdio.h>

#include <string>
#include <optional>

#include "std_comp.hh"

class PosixExtRegex
{
public:
  //
  // default ctor
  PosixExtRegex ();

  //
  // move ctor
  PosixExtRegex (PosixExtRegex &&move_ctor_from);

  //
  // construct from std::string
  PosixExtRegex (const std::string &ptn);

  //
  // move assign
  PosixExtRegex &operator= (PosixExtRegex &&move_assign_from);

  // dtor
  ~PosixExtRegex ();

  inline bool is_pattern_accept () noexcept { return this->stat_comp == 0; }

  std::string get_err () noexcept;
  std::optional<bool> is_match (const std::string &str) noexcept;
  string ptn () const noexcept { return __ptn; }

private:
  int stat_comp;
  regex_t *re;
  string __ptn;
};

#endif // _POSIX_REGEX_H
