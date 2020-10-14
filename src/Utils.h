#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

// enable log messages
#define VERBOSE
// enable debug mode
// #define DEBUG_ENABLE

namespace utils
{

  using std::cout;
  using std::string;
  using std::to_string;

  /*
   * color_code refer to https://www.wikiwand.com/en/ANSI_escape_code
  */

  void debug(const string str)
  {
#if defined(VERBOSE) && defined(DEBUG_ENABLE)
    string output;
    output = "\033[35m[DEBUG]\033[0m " + str;
    cout << output << '\n';
#endif
  }
  void info(const string str)
  {
#if defined(VERBOSE)
    string output;
    output = "\033[32m[INFO]\033[0m " + str;
    cout << output << '\n';
#endif
  }
  void warning(const string str)
  {
#if defined(VERBOSE)
    string output;
    output = "\033[93m[WARN]\033[0m " + str;
    cout << output << '\n';
#endif
  }
  void error(const string str)
  {
#if defined(VERBOSE)
    string output;
    output = "\033[91m[ERROR]\033[0m " + str;
    cout << output << '\n';
#endif
  }

}; // namespace utils

#endif