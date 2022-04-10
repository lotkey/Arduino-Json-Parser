#include "stringpp.hpp"

#include <sstream>
#include <string>

namespace stringpp {
int atoi(const std::string &str) {
#ifdef _USE_STRINGPP
   std::stringstream ss;
   int i;
   ss << str;
   ss >> i;
   return i;
#else
   return std::atoi(str.c_str());
#endif
}

float atof(const std::string &str) {
#ifdef _USE_STRINGPP
   std::stringstream ss;
   float f;
   ss << str;
   ss >> f;
   return f;
#else
   return std::atof(str.c_str());
#endif
}
} // namespace stringpp