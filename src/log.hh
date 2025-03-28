#ifndef hh_log
#define hh_log

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdint.h>
#include <assert.h>
#include <concepts>
#include <time.h>
#include <iterator>

#include "stat.hh"
#include "std_comp.hh"

#ifndef __TU__
#define __TU__ "?TU"
#endif

#define tulogfe(...) tgf::logfe (__TU__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__)
#define tulogfi(...) tgf::logfi (__TU__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__)
#define tulogfw(...) tgf::logfw (__TU__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__)
#define tulogfd(...) tgf::logfd (__TU__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__)

#define tulogfe_cg(cg, ...)                                                    \
  tgf::logfe_cg (cg, __TU__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__)
#define tulogfi_cg(cg, ...)                                                    \
  tgf::logfi_cg (cg, __TU__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__)
#define tulogfw_cg(cg, ...)                                                    \
  tgf::logfw_cg (cg, __TU__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__)
#define tulogfd_cg(cg, ...)                                                    \
  tgf::logfd_cg (cg, __TU__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__)

namespace tgf {

// ------------------------------- - -------------------------------

enum LogLv
{
  ERROR = 1,
  WARN = 2,
  INFO = 3,
  DEBUG = 4,
};

std::ostream &
operator<< (std::ostream &os, LogLv lv);

extern LogLv g_loglv;

template <typename OS, class T>
void
print_all (OS &os, T t)
{
  os << t;
}

template <typename OS, class T, class... Args>
void
print_all (OS &os, T t, Args... args)
{
  os << t;
  os << ",";
  print_all (os, args...);
}

inline std::string
curr_tstamp ()
{
  std::time_t time = std::time ({});
  char time_str[std::size ("2025-03-11-T18:17:32+0000")];
  std::strftime (std::data (time_str), std::size (time_str), "%FT%TZ%z",
		 std::localtime (&time));
  return std::string (time_str);
}

template <typename S, class... Args>
requires std::derived_from<S, std::ostream> void inline stream_log (
  S &os, LogLv lv, const Args &...args)
{
  if (g_loglv < lv)
    return;

  os << "[tgf ";
  os << lv;
  os << "] ";
  os << curr_tstamp ();
  os << " ";
  print_all (os, args...);
  os << std::endl;
  os << std::flush;
}

// impl should be identical to the above lvalue ref one
// TODO: reduce repeatition by ajusting template arg
template <typename S, class... Args>
requires derived_from<S, std::ostream> inline void
stream_log (S &&os, LogLv lv, const Args &...args)
{
  if (g_loglv < lv)
    return;

  os << "[tgf ";
  os << lv;
  os << "] ";
  os << curr_tstamp ();
  os << " ";
  print_all (os, args...);
  os << std::endl;
  os << std::flush;
}

// ------------------------------- - -------------------------------

/*
 * Log in stdout, error level
 */
template <class... Args>
void
loge (const Args &...args)
{
  stream_log (std::cout, LogLv::ERROR, args...);
}

/*
 * Log in stdout, warn level
 */
template <class... Args>
void
logw (const Args &...args)
{
  stream_log (std::cout, LogLv::WARN, args...);
}

/*
 * Log in stdout, info level
 */
template <class... Args>
void
logi (const Args &...args)
{
  stream_log (std::cout, LogLv::INFO, args...);
}

/*
 * Log in stdout, debug level
 */
template <class... Args>
void
logd (const Args &...args)
{
  stream_log (std::cout, LogLv::DEBUG, args...);
}

// -------------------------- log in file --------------------------

inline ofstream
_get_log_file (u8 ch_grp = 0)
{
  fs::path p = tgfstat::userdata.path_log_parent ();
  bool is_child = tgfstat::im_child.load (mo::relaxed);

  if (is_child)
    {
      switch (ch_grp)
	{
	case 1:
	  p = tgfstat::userdata.path_log_child_coll ();
	  break;
	default:
	  p = tgfstat::userdata.path_log_child ();
	  break;
	}
    }
  bool needs_trunc = is_too_large (p);
  ofstream s (p.c_str (), needs_trunc ? ios::trunc : ios::app);
  return s;
}

/*
 * Log in a file, error level
 */
template <class... Args>
inline void
logfe (const Args &...args)
{
  stream_log (_get_log_file (), LogLv::ERROR, args...);
}

/*
 * Log in a file, warn level
 */
template <class... Args>
void
logfw (const Args &...args)
{
  stream_log (_get_log_file (), LogLv::WARN, args...);
}

/*
 * Log in a file, info level
 */
template <class... Args>
void
logfi (const Args &...args)
{
  stream_log (_get_log_file (), LogLv::INFO, args...);
}

/*
 * Log in a file, debug level
 */
template <class... Args>
void
logfd (const Args &...args)
{
  stream_log (_get_log_file (), LogLv::DEBUG, args...);
}

/*
 * Log in a file, error level
 */
template <class... Args>
inline void
logfe_cg (u8 ch_grp, const Args &...args)
{
  stream_log (_get_log_file (ch_grp), LogLv::ERROR, args...);
}

/*
 * Log in a file, warn level
 */
template <class... Args>
void
logfw_cg (u8 ch_grp, const Args &...args)
{
  stream_log (_get_log_file (ch_grp), LogLv::WARN, args...);
}

/*
 * Log in a file, info level
 */
template <class... Args>
void
logfi_cg (u8 ch_grp, const Args &...args)
{
  stream_log (_get_log_file (ch_grp), LogLv::INFO, args...);
}

/*
 * Log in a file, debug level
 */
template <class... Args>
void
logfd_cg (u8 ch_grp, const Args &...args)
{
  stream_log (_get_log_file (ch_grp), LogLv::DEBUG, args...);
}

// ------------------------------- AUX -------------------------------

inline std::string
as_hex_list (const std::vector<uint8_t> &arr)
{
  std::ostringstream oss;
  oss << "[";
  for (size_t i = 0; i < arr.size (); ++i)
    {
      oss << "0x" << std::hex << std::setw (2) << std::setfill ('0')
	  << static_cast<int> (arr[i]);
      oss << ",";
    }
  oss << "]";
  return oss.str ();
}

} // namespace tgf

#endif
