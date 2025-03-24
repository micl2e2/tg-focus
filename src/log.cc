#define __TU__ "log"

#include <iostream>
#include "log.hh"

namespace tgf {

// Global Log Level Indicator
LogLv g_loglv = LogLv::INFO;

std::ostream &
operator<< (std::ostream &os, LogLv lv)
{
  switch (lv)
    {
    case LogLv::ERROR:
      os << "E";
      break;
    case LogLv::WARN:
      os << "W";
      break;
    case LogLv::INFO:
      os << "I";
      break;
    case LogLv::DEBUG:
      os << "D";
      break;
    }
  return os;
}

} // namespace tgf
