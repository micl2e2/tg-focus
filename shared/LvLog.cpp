#include <iostream>
#include "LvLog.hpp"

LogLv gLogLv = LogLv::INFO;

std::ostream &
operator<< (std::ostream &os, LogLv lv)
{
  switch (lv)
    {
    case LogLv::ERROR:
      os << "ERRO";
      break;
    case LogLv::WARNING:
      os << "WARN";
      break;
    case LogLv::INFO:
      os << "INFO";
      break;
    case LogLv::DEBUG:
      os << " DBG";
      break;
    }
  return os;
}
