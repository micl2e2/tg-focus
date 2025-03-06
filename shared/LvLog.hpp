#ifndef _LV_LOG_H
#define _LV_LOG_H

#include <iostream>

enum LogLv
{
  ERROR = 1,
  WARNING = 2,
  INFO = 3,
  DEBUG = 4,
};

std::ostream &
operator<< (std::ostream &os, LogLv lv);

extern LogLv gLogLv;

template <class T>
void
print_all (T t)
{
  std::cout << t;
}

template <class T, class... Args>
void
print_all (T t, Args... args)
{
  std::cout << t;
  print_all (args...);
}

template <class... Args>
void
lvlog (LogLv lv, const Args &...args)
{
  if (gLogLv >= lv)
    {
      std::cout << "[tgf ";
      std::cout << lv;
      std::cout << "] ";
      print_all (args...);
      std::cout << std::endl;
      std::cout << std::flush;
    }
}

#endif
