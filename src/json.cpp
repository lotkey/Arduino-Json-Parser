#include "json.hpp"
#include "json_object.hpp"

#include <string>

namespace json {
Json::Json(const std::string &json)
    : m_input(json), m_json(parse(split_into_tokens(json))) {}

std::string Json::toStdString() const { return m_json.toString(); }

const JsonObject &Json::operator[](const std::string &key) const {
  return m_json[key];
}

const JsonObject &Json::operator[](int index) const { return m_json[index]; }

JsonObject &Json::operator[](const std::string &key) { return m_json[key]; }

JsonObject &Json::operator[](int index) { return m_json[index]; }
} // namespace json