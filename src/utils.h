#ifndef UTILS_H
#define UTILS_H
// enable debug mode
#define DEBUG_MODE

#include <iostream>
#include <string>

namespace utils
{

  using std::cout;
  using std::string;

  void debug(string str)
  {
#ifdef DEBUG_MODE
    cout << "[DEBUG] " << str << '\n';
#endif
  }
  void info(string str)
  {
#ifdef DEBUG_MODE
    cout << "[INFO] " << str << '\n';
#endif
  }
  void warning(string str)
  {
#ifdef DEBUG_MODE
    cout << "[WARN] " << str << '\n';
#endif
  }
  void error(string str)
  {
#ifdef DEBUG_MODE
    cout << "[ERROR] " << str << '\n';
#endif
  }

}; // namespace utils

#endif