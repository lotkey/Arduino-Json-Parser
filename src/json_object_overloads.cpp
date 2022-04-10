#include "json_object.hpp"

namespace json {
#pragma region Getters
JsonObject::operator int() const { return asInt(); }

JsonObject::operator bool() const { return asBool(); }

JsonObject::operator float() const { return asFloat(); }

JsonObject::operator std::string() const { return asStdString(); }

JsonObject::operator std::vector<JsonObject>() const { return asArray(); }

JsonObject::operator std::pair<std::string, JsonObject>() const {
  return asProperty();
}

const JsonObject &JsonObject::operator[](int index) const {
  if (m_type == Type::Array) {
    return asArray()[index];
  } else {
    throw std::runtime_error("Can only index arrays with integers");
  }
}

const JsonObject &JsonObject::operator[](const char *key) const {
  if (m_type == Type::Object) {
    return asObject().at(key);
  } else {
    throw std::runtime_error("Can only index objects with strings");
  }
}

const JsonObject &JsonObject::operator[](const std::string &key) const {
  if (m_type == Type::Object) {
    return asObject().at(key);
  } else {
    throw std::runtime_error("Can only index objects with strings");
  }
}
#pragma endregion

#pragma region Setters
JsonObject &JsonObject::operator[](int index) {
  if (m_type == Type::Array) {
    return asArray()[index];
  } else {
    throw std::runtime_error("Can only index arrays with integers");
  }
}

JsonObject &JsonObject::operator[](const char *key) {
  if (m_type == Type::Object) {
    return asObject()[key];
  } else {
    throw std::runtime_error("Can only index objects with strings");
  }
}

JsonObject &JsonObject::operator[](const std::string &key) {
  if (m_type == Type::Object) {
    return asObject()[key];
  } else {
    throw std::runtime_error("Can only index objects with strings");
  }
}
#pragma endregion

} // namespace json