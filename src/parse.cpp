#include "json.hpp"
#include "stringpp.hpp"

#include <algorithm>

namespace json {
JsonObject Json::parse(std::vector<std::string> tokens) {
  return parseValue(tokens);
}

JsonObject Json::parseValue(std::vector<std::string> &tokens) {
  if (tokens.front() == "{") {
    return parseObject(tokens);
  } else if (tokens.front() == "[") {
    return parseArray(tokens);
  } else if (tokens.front() == "null") {
    tokens.erase(tokens.begin());
    return JsonObject();
  } else if (tokens.front() == "true" || tokens.front() == "false") {
    return parseBool(tokens);
  } else if (tokens.front()[0] == '"') {
    return parseString(tokens);
  } else {
    return parseNumber(tokens);
  }
}

JsonObject Json::parseArray(std::vector<std::string> &tokens) {
  if (tokens.empty()) {
    throw std::runtime_error("Parsing error: Expecting ARRAY, got $end.");
  }
  if (tokens.front().empty()) {
    throw std::runtime_error("Parsing error: Expecting [, got $empty.");
  }
  if (tokens.front() != "[") {
    throw std::runtime_error("Parsing error: Expecting [, got \"" +
                             tokens.front() + "\".");
  }

  std::vector<JsonObject> values;
  int count = 0;

  // Remove the left bracket
  tokens.erase(tokens.begin());
  while (tokens.front() != "]") {
    if (tokens.front() == ",") {
      if (count == 0) {
        throw std::runtime_error(
            "Parsing error: Parsing ARRAY, unexpected COMMA.");
      }
      tokens.erase(tokens.begin());
    } else {
      auto value = parseValue(tokens);
      values.push_back(value);
    }
    count++;
  }

  if (tokens.empty()) {
    throw std::runtime_error(
        "Parsing error: Parsing ARRAY. Expecting ], got $end.");
  }
  if (tokens.front().empty()) {
    throw std::runtime_error(
        "Parsing error: Parsing ARRAY. Expecting ], got $empty.");
  }
  if (tokens.front() != "]") {
    throw std::runtime_error(
        "Parsing error: Parsing ARRAY. Expecting ], got \"" + tokens.front() +
        "\".");
  }
  // Remove the right bracket
  tokens.erase(tokens.begin());
  return JsonObject(values);
}

JsonObject Json::parseObject(std::vector<std::string> &tokens) {
  if (tokens.empty()) {
    throw std::runtime_error("Parsing error: Expecting OBJECT, got $end.");
  }
  if (tokens.front().empty()) {
    throw std::runtime_error("Parsing error: Expecting {, got $empty.");
  }
  if (tokens.front() != "{") {
    throw std::runtime_error("Parsing error: Expecting {, got \"" +
                             tokens.front() + "\".");
  }

  std::map<std::string, JsonObject> properties;
  int count = 0;

  // Remove the left curly bracket
  tokens.erase(tokens.begin());
  while (tokens.front() != "}") {
    if (tokens.front() == ",") {
      if (count == 0) {
        throw std::runtime_error(
            "Parsing error: Parsing OBJECT, unexpected COMMA.");
      }
      tokens.erase(tokens.begin());
    } else {
      auto property = parseProperty(tokens);
      auto str = std::get<0>(property.asProperty());
      auto value = std::get<1>(property.asProperty());
      properties.insert({str, value});
    }
    count++;
  }

  if (tokens.empty()) {
    throw std::runtime_error(
        "Parsing error: Parsing OBJECT. Expecting }, got $end.");
  }
  if (tokens.front().empty()) {
    throw std::runtime_error(
        "Parsing error: Parsing OBJECT. Expecting }, got $empty.");
  }
  if (tokens.front() != "}") {
    throw std::runtime_error(
        "Parsing error: Parsing OBJECT. Expecting }, got \"" + tokens.front() +
        "\".");
  }

  // Remove the right curly bracket
  tokens.erase(tokens.begin());
  return JsonObject(properties);
}

JsonObject Json::parseProperty(std::vector<std::string> &tokens) {
  if (tokens.empty()) {
    throw std::runtime_error("Parsing error: Expecting PROPERTY, got $end.");
  }
  if (tokens.size() < 3) {
    std::string error =
        "Parsing error: Expecting PROPERTY, got too few tokens: ";
    for (const auto &token : tokens) {
      error += "\"" + token + "\", ";
    }
    throw std::runtime_error(error);
  }
  if (tokens[1] != ":") {
    throw std::runtime_error(
        "Parsing error: Parsing PROPERTY. Expecting COLON, got \"" + tokens[1] +
        "\".");
  }

  JsonObject str = parseString(tokens);
  // Remove the colon from the tokens
  tokens.erase(tokens.begin());
  JsonObject value = parseValue(tokens);
  return JsonObject(std::pair<std::string, JsonObject>(str, value));
}

JsonObject Json::parseBool(std::vector<std::string> &tokens) {
  if (tokens.empty()) {
    throw std::runtime_error("Parsing error: Expecting BOOL, got $end.");
  }
  if (tokens.front().empty()) {
    throw std::runtime_error("Parsing error: Expecting BOOL, got $empty.");
  }
  if (tokens.front() != "true" || tokens.front() != "false") {
    throw std::runtime_error("Parsing error: Expecting BOOL, got \"" +
                             tokens.front() + "\".");
  }

  bool b = tokens.front() == "true";
  tokens.erase(tokens.begin());
  return JsonObject(b);
}

JsonObject Json::parseNumber(std::vector<std::string> &tokens) {
  if (tokens.empty()) {
    throw std::runtime_error(
        "Parsing error: Expecting INT or FLOAT, got $end.");
  }
  if (tokens.front().empty()) {
    throw std::runtime_error(
        "Parsing error: Expecting INT or FLOAT, got $empty.");
  }
  bool hasNoDigits =
      tokens.front().find_first_of("0123456789") == std::string::npos;
  if (hasNoDigits) {
    throw std::runtime_error("Parsing error: Expecting INT or FLOAT, got \"" +
                             tokens.front() + "\".");
  }
  auto invalidCharPos = tokens.front().find_first_not_of("0123456789-.");
  if (invalidCharPos != std::string::npos) {
    char c = tokens.front()[invalidCharPos];
    throw std::runtime_error(
        "Parsing error: Expecting INT or FLOAT, found invalid character '" +
        std::string(1, c) + "': \"" + tokens.front() + "\".");
  }
  auto lastDashIndex = tokens.front().find_last_of("-");
  if (lastDashIndex > 0 && lastDashIndex != std::string::npos) {
    throw std::runtime_error(
        "Parsing error: Expecting INT or FLOAT, invalid placement of '-': \"" +
        tokens.front() + "\".");
  }
  auto lastDecimalPosition = tokens.front().find_last_of(".");

  if (lastDecimalPosition != std::string::npos && lastDecimalPosition > 0 &&
      (tokens.front()[lastDecimalPosition - 1] < '0' ||
       tokens.front()[lastDecimalPosition - 1] > '9')) {
    throw std::runtime_error("Parsing error: Expecting INT or FLOAT. Decimal "
                             "must come after a digit: \"" +
                             tokens.front() + "\".");
  }
  auto decimalCount =
      std::count(tokens.front().begin(), tokens.front().end(), '.');
  if (decimalCount > 1) {
    throw std::runtime_error(
        "Parsing error: Expecting INT or FLOAT. Too many decimal points: \"" +
        tokens.front() + "\".");
  }

  if (tokens.front().find('.') == std::string::npos) {
    int i = stringpp::atoi(tokens.front().c_str());
    tokens.erase(tokens.begin());
    return JsonObject(i);
  } else {
    float f = stringpp::atof(tokens.front().c_str());
    tokens.erase(tokens.begin());
    return JsonObject(f);
  }
}

JsonObject Json::parseString(std::vector<std::string> &tokens) {
  if (tokens.empty()) {
    throw std::runtime_error("Parsing error: Expecting STRING, got $end.");
  }
  if (tokens.front().length() < 2) {
    throw std::runtime_error("Parsing error: Expecting STRING, got $empty.");
  }
  if (!(tokens.front()[0] == '"' &&
        tokens.front()[tokens.front().length() - 1] == '"')) {
    throw std::runtime_error("Parsing error: Expecting STRING, got \"" +
                             tokens.front() + "\".");
  }

  std::string without_quotes =
      tokens.front().substr(1, tokens.front().length() - 2);
  tokens.erase(tokens.begin());
  return JsonObject(without_quotes);
}
} // namespace json