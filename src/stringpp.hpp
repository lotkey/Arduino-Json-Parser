#pragma once

#include <sstream>
#include <string>

// Uncomment the line below if you get 'to_string'/'atof'/'atoi' is not a member
// of 'std'.
// clang-format off
// #define _USE_STRINGPP
// clang-format on

namespace stringpp {
template <typename T> std::string to_string(const T &t) {
#ifdef _USE_STRINGPP
   std::stringstream ss;
   std::string str;
   ss << t;
   ss >> str;
   return str;
#else
   return std::to_string(t);
#endif
}

int atoi(const std::string &str);
float atof(const std::string &str);
} // namespace stringpp