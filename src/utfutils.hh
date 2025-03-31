#ifndef hh_utfutils
#define hh_utfutils

#include "std_comp.hh"

#include <string>
#include <string.h>
#include <stddef.h>

size_t
get_c16_len (const string &utf8s);

vector<char16_t>
get_c16_seq (const string &utf8s);

#endif
