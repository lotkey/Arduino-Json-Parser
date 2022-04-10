#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace json {
class JsonObject {
public:
  enum class Type { Array, Int, Bool, Float, String, Property, Object, Null };

  // Construction/Destruction
  JsonObject();
  JsonObject(int);
  JsonObject(bool);
  JsonObject(float);
  JsonObject(double);
  JsonObject(const char *);
  JsonObject(const std::string &);
  JsonObject(const std::vector<JsonObject> &);
  JsonObject(const std::map<std::string, JsonObject> &);
  JsonObject(const std::pair<std::string, JsonObject> &);
  JsonObject(const JsonObject &);
  JsonObject(JsonObject &&);
  ~JsonObject();
  void operator=(const JsonObject &);
  void operator=(JsonObject &&);

  std::string toString() const;
  bool is(Type) const;
  Type type() const;
  int size() const;
  bool contains(const std::string &) const;

#pragma region json_object_overloads.cpp
  // Getters
  operator int() const;
  operator bool() const;
  operator float() const;
  operator std::string() const;
  operator std::pair<std::string, JsonObject>() const;
  operator std::vector<JsonObject>() const;
  operator std::map<std::string, JsonObject>() const;
  const JsonObject &operator[](int) const;
  const JsonObject &operator[](const char *) const;
  const JsonObject &operator[](const std::string &) const;

  // Setters
  JsonObject &operator[](int);
  JsonObject &operator[](const char *);
  JsonObject &operator[](const std::string &);
#pragma endregion

#pragma region json_object_accessors.cpp
  // Getters
  int asInt() const;
  bool asBool() const;
  float asFloat() const;
  const char *asCString() const;
  const std::string &asStdString() const;
  const std::pair<std::string, JsonObject> &asProperty() const;
  const std::vector<JsonObject> &asArray() const;
  const std::map<std::string, JsonObject> &asObject() const;

  // Setters
  int &asInt();
  float &asFloat();
  bool &asBool();
  std::string &asStdString();
  std::pair<std::string, JsonObject> &asProperty();
  std::vector<JsonObject> &asArray();
  std::map<std::string, JsonObject> &asObject();
#pragma endregion

private:
  Type m_type;
  void *m_data = nullptr;

  std::string toString(int depth) const;
  void freeMemory();
  void copyTo(JsonObject &) const;
  void moveTo(JsonObject &);
  template <typename T> const T &as() const;
  template <typename T> T &as();
};

template <typename T> const T &JsonObject::as() const { return *(T *)m_data; }

template <typename T> T &JsonObject::as() { return *(T *)m_data; }
} // namespace json