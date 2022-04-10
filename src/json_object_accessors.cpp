#include "json_object.hpp"

namespace json {
#pragma region Getters
int JsonObject::asInt() const {
  switch (m_type) {
  case Type::Int:
    return as<int>();
  case Type::Bool:
    return as<bool>();
  case Type::Float:
    return as<float>();
  default:
    throw std::runtime_error("Bad cast to int.");
  };
}

bool JsonObject::asBool() const {
  switch (m_type) {
  case Type::Int:
    return as<int>();
  case Type::Bool:
    return as<bool>();
  case Type::Float:
    return as<float>();
  default:
    throw std::runtime_error("Bad cast to bool.");
  }
}

float JsonObject::asFloat() const {
  switch (m_type) {
  case Type::Int:
    return as<int>();
  case Type::Bool:
    return as<bool>();
  case Type::Float:
    return as<float>();
  default:
    throw std::runtime_error("Bad cast to float.");
  };
}

const char *JsonObject::asCString() const {
  switch (m_type) {
  case Type::String:
    return as<std::string>().c_str();
  default:
    throw std::runtime_error("Bad cast to C-string");
  }
}

const std::string &JsonObject::asStdString() const {
  switch (m_type) {
  case Type::String:
    return as<std::string>();
  default:
    throw std::runtime_error("Bad cast to std::string");
  }
}

const std::pair<std::string, JsonObject> &JsonObject::asProperty() const {
  switch (m_type) {
  case Type::Property:
    return as<std::pair<std::string, JsonObject>>();
  default:
    throw std::runtime_error("Bad cast to property");
  }
}

const std::vector<JsonObject> &JsonObject::asArray() const {
  switch (m_type) {
  case Type::Array:
    return as<std::vector<JsonObject>>();
  default:
    throw std::runtime_error("Bad cast to array");
  }
}
#pragma endregion

#pragma region Setters
int &JsonObject::asInt() {
  switch (m_type) {
  case Type::Int:
    return as<int>();
  default:
    throw std::runtime_error("Bad cast to int.");
  };
}

bool &JsonObject::asBool() {
  switch (m_type) {
  case Type::Bool:
    return as<bool>();
  default:
    throw std::runtime_error("Bad cast to bool.");
  }
}

float &JsonObject::asFloat() {
  switch (m_type) {
  case Type::Float:
    return as<float>();
  default:
    throw std::runtime_error("Bad cast to float.");
  };
}

std::string &JsonObject::asStdString() {
  switch (m_type) {
  case Type::String:
    return as<std::string>();
  default:
    throw std::runtime_error("Bad cast to std::string");
  }
}

std::pair<std::string, JsonObject> &JsonObject::asProperty() {
  switch (m_type) {
  case Type::Property:
    return as<std::pair<std::string, JsonObject>>();
  default:
    throw std::runtime_error("Bad cast to property");
  }
}

std::vector<JsonObject> &JsonObject::asArray() {
  switch (m_type) {
  case Type::Array:
    return as<std::vector<JsonObject>>();
  default:
    throw std::runtime_error("Bad cast to array");
  }
}

std::map<std::string, JsonObject> &JsonObject::asObject() {
  switch (m_type) {
  case Type::Object:
    return as<std::map<std::string, JsonObject>>();
  default:
    throw std::runtime_error("Bad cast to object.");
  }
}
#pragma endregion
} // namespace json