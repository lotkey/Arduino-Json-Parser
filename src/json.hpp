#pragma once

#include "json_object.hpp"

#include <string>
#include <vector>

namespace json {

class Json {
public:
  Json(const std::string &json);
  const JsonObject &operator[](const std::string &) const;
  const JsonObject &operator[](int) const;
  JsonObject &operator[](const std::string &);
  JsonObject &operator[](int);
  std::string toStdString() const;

private:
  std::string m_input;
  JsonObject m_json;

#pragma region lex.cpp
  static size_t find_first_of_any(const std::string &,
                                  const std::vector<char> &delimiters);
  static std::vector<std::string> split_into_tokens(const std::string &);
  static std::vector<std::string> split_by_string_literals(const std::string &);
  static std::vector<std::string> split(
      std::string,
      const std::vector<char> &separators = {' ', '\t', '\v', '\f', '\n', '\r'},
      bool keepSeparators = false);
#pragma endregion
#pragma region parse.cpp
  static JsonObject parse(std::vector<std::string> tokens);
  static JsonObject parseValue(std::vector<std::string> &tokens);
  static JsonObject parseArray(std::vector<std::string> &tokens);
  static JsonObject parseObject(std::vector<std::string> &tokens);
  static JsonObject parseProperty(std::vector<std::string> &tokens);
  static JsonObject parseBool(std::vector<std::string> &tokens);
  static JsonObject parseNumber(std::vector<std::string> &tokens);
  static JsonObject parseString(std::vector<std::string> &tokens);
#pragma endregion
};
} // namespace json