#ifndef _POSIX_REGEX_H
#define _POSIX_REGEX_H 1

#include <assert.h>
#include <stdint.h>
#include <limits.h>
#include <regex.h>
#include <stddef.h>
#include <stdio.h>

#include <string>
#include <optional>

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

private:
  int stat_comp;
  regex_t *re;
};

#endif // _POSIX_REGEX_H
