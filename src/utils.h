#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

namespace utils {

  using std::cout;
  using std::string;

  void debug(string str){
    cout << "[DEBUG] " << str << '\n';
  }
  void info(string str){
    cout << "[INFO] " << str << '\n';
  }
  void warning(string str){
    cout << "[WARN] " << str << '\n';
  }
  void error(string str) {
    cout << "[ERROR] " << str << '\n';
  }

};

#endif