#ifndef hh_std_comp
#define hh_std_comp

#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <atomic>
#include <filesystem>
#include <optional>
#include <thread>

// ------------------------------- - -------------------------------

// NOTE: almost every file will include this one, careful about custom headers
// #include "common.hh"
//
// std
// -> std_comp
//   -> stat
//     -> log
//       -> common
//         -> others
//
// i.e. log.hh can include stat.hh, cannot include common.hh

// ------------------------------- - -------------------------------

using namespace std;

namespace fs = filesystem;
namespace chron = chrono;

using path = std::filesystem::path;
using mo = std::memory_order;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using SockAddrPtr = struct sockaddr *;
using UniSockAddr = struct sockaddr_un;
using UniSockAddrPtr = struct sockaddr_un *;
using InSockAddr = struct sockaddr_in;
using InSockAddrPtr = struct sockaddr_in *;

// ------------------------------- - -------------------------------

template <typename To, typename From>
To
scast (From from)
{
  return static_cast<To> (from);
}

template <typename To, typename From>
To
rcast (From from)
{
  return reinterpret_cast<To> (from);
}

template <typename T>
inline std::ostream &
operator<< (std::ostream &os, std::vector<T> t)
{
  for (auto ele : t)
    os << ele << ",";

  return os;
}

inline std::ostream &
operator<< (std::ostream &os, std::tuple<int, int> t)
{
  os << "<" << get<0> (t) << "," << get<1> (t) << ">";
  return os;
}

namespace tgf {

inline void
exitMain (u8 ec)
{
  exit (1);
}

} // namespace tgf

#endif
