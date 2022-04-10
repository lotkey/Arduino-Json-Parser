#include "json_object.hpp"
#include "stringpp.hpp"

#include <functional>
#include <string>
#include <tuple>
#include <vector>

namespace json {
JsonObject::JsonObject() : m_type(Type::Null), m_data(nullptr) {}

JsonObject::JsonObject(int i) : m_type(Type::Int), m_data(new int(i)) {}

JsonObject::JsonObject(bool b) : m_type(Type::Bool), m_data(new bool(b)) {}

JsonObject::JsonObject(float f) : m_type(Type::Float), m_data(new float(f)) {}

JsonObject::JsonObject(double d) : m_type(Type::Float), m_data(new float(d)) {}

JsonObject::JsonObject(const char *s)
    : m_type(Type::String), m_data(new std::string(s)) {}

JsonObject::JsonObject(const std::string &s)
    : m_type(Type::String), m_data(new std::string(s)) {}

JsonObject::JsonObject(const std::vector<JsonObject> &a)
    : m_type(Type::Array), m_data(new std::vector<JsonObject>(a)) {}

JsonObject::JsonObject(const std::map<std::string, JsonObject> &properties)
    : m_type(Type::Object),
      m_data(new std::map<std::string, JsonObject>(properties)) {}

JsonObject::JsonObject(const std::pair<std::string, JsonObject> &property)
    : m_type(Type::Property),
      m_data(new std::pair<std::string, JsonObject>(property)) {}

JsonObject::JsonObject(const JsonObject &jsonObject) {
  jsonObject.copyTo(*this);
}

JsonObject::JsonObject(JsonObject &&jsonObject) { jsonObject.moveTo(*this); }

JsonObject::~JsonObject() { freeMemory(); }

void JsonObject::operator=(const JsonObject &jsonObject) {
  jsonObject.copyTo(*this);
}

void JsonObject::operator=(JsonObject &&jsonObject) {
  jsonObject.moveTo(*this);
}

std::string JsonObject::toString() const { return toString(0); }

std::string JsonObject::toString(int depth) const {
  std::string tab = "   ";
  std::string indentation;
  for (int i = 0; i < depth; i++) {
    indentation += tab;
  }

  switch (m_type) {
  case Type::Int: {
    int data = asInt();
    return indentation + stringpp::to_string(data);
  }
  case Type::Bool: {
    bool data = asBool();
    return indentation + (data ? "true" : "false");
  }
  case Type::Float: {
    float data = asFloat();
    return indentation + stringpp::to_string(data);
  }
  case Type::Null: {
    return indentation + "null";
  }
  case Type::String: {
    std::string data = asStdString();
    return indentation + "\"" + data + "\"";
  }
  case Type::Property: {
    auto property = asProperty();
    auto label = std::get<0>(property);
    auto value = std::get<1>(property);

    if (value.is(Type::Array) || value.is(Type::Object)) {
      std::string str = indentation + "\"" + label + "\": ";
      std::string valueStr = value.toString(depth);

      while (valueStr.front() == ' ') {
        valueStr.erase(valueStr.begin());
      }

      str += valueStr;
      return str;
    } else {
      std::string str = indentation + "\"" + label + "\": " + value.toString();
      return str;
    }
  }
  case Type::Array: {
    std::vector<JsonObject> arr = asArray();
    std::string str = indentation + "[\n";
    for (int i = 0; i < arr.size(); i++) {
      str += arr[i].toString(depth + 1);
      if (i != arr.size() - 1) {
        str += ",";
      }
      str += "\n";
    }
    str += indentation + "]";
    return str;
  }
  case Type::Object: {
    auto data = asObject();
    std::string str = indentation + "{\n";
    for (const auto &pair : data) {
      auto label = std::get<0>(pair);
      auto object = std::get<1>(pair);
      JsonObject property = JsonObject(std::make_pair(label, object));
      str += property.toString(depth + 1) + ",\n";
    }

    if (!data.empty()) {
      // remove the last comma
      str.erase(str.end() - 2);
    }

    str += indentation + "}";
    return str;
  }
  default: {
    return "Incomplete toString";
  }
  }
}

void JsonObject::copyTo(JsonObject &json) const {
  json.freeMemory();
  json.m_type = m_type;
  // JsonObject(jsonObject);
  switch (m_type) {
  case Type::Int:
    json.m_data = new int(asInt());
    break;
  case Type::Bool:
    json.m_data = new bool(asBool());
    break;
  case Type::Float:
    json.m_data = new float(asFloat());
    break;
  case Type::Null:
    json.m_data = nullptr;
    break;
  case Type::String:
    json.m_data = new std::string(asStdString());
    break;
  case Type::Property:
    json.m_data = new std::pair<std::string, JsonObject>(asProperty());
    break;
  case Type::Object: {
    json.m_data = new std::map<std::string, JsonObject>();
    for (const auto &pair : asObject()) {
      auto label = std::get<0>(pair);
      auto object = std::get<1>(pair);
      ((std::map<std::string, JsonObject> *)json.m_data)
          ->insert({label, object});
    }
    break;
  }
  case Type::Array: {
    json.m_data = new std::vector<JsonObject>();
    for (const auto &object : asArray()) {
      ((std::vector<JsonObject> *)json.m_data)->push_back(object);
    }
    break;
  }
  };
}

void JsonObject::moveTo(JsonObject &json) {
  json.freeMemory();
  json.m_data = m_data;
  json.m_type = m_type;
  m_data = nullptr;
  m_type = Type::Null;
}

void JsonObject::freeMemory() {
  if (m_data) {
    switch (m_type) {
    case Type::Int:
      delete (int *)m_data;
      break;
    case Type::Bool:
      delete (bool *)m_data;
      break;
    case Type::Array:
      delete (std::vector<JsonObject> *)m_data;
      break;
    case Type::Float:
      delete (float *)m_data;
      break;
    case Type::Null:
      break;
    case Type::Object:
      delete (std::map<std::string, JsonObject> *)m_data;
      break;
    case Type::Property:
      delete (std::pair<std::string, JsonObject> *)m_data;
      break;
    case Type::String:
      delete (std::string *)m_data;
      break;
    }
  }
}

const std::map<std::string, JsonObject> &JsonObject::asObject() const {
  switch (m_type) {
  case Type::Object:
    return as<std::map<std::string, JsonObject>>();
  default:
    throw std::runtime_error("Bad cast to object.");
  }
}

bool JsonObject::is(Type t) const { return m_type == t; }

JsonObject::Type JsonObject::type() const { return m_type; }
} // namespace json