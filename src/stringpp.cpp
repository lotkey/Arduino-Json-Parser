#include "stringpp.hpp"

#include <sstream>

namespace stringpp {
int atoi(const std::string &str) {
  std::stringstream ss;
  int i;
  ss << str;
  ss >> i;
  return i;
}

float atof(const std::string &str) {
  std::stringstream ss;
  float f;
  ss << str;
  ss >> f;
  return f;
}
} // namespace stringpp