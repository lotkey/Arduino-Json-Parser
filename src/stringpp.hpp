#pragma once

#include <sstream>
#include <string>

namespace stringpp {
template <typename T> std::string to_string(const T &t) {
  std::stringstream ss;
  std::string str;
  ss << t;
  ss >> str;
  return str;
}

int atoi(const std::string &str);
float atof(const std::string &str);
} // namespace stringpp