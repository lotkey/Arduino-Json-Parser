# Arduino JSON Parser

I found that the ArduinoJSON library was annoying to work with, so I made my own. It uses recursive descent to parse JSON.

## Including

The only file you need to include is `json.hpp`.


## Using


### Reading

To read JSON, construct a json::Json object with the JSON passed in as an argument.

```c++
#include "json.hpp"

int main() {
    json::Json json("{ \"test\": 1, \"test2\": [ 0, 1, 2, 3 ] }");
}
```

### Accessing

To access JSON elements you can use the overloaded `[]` operator. To access a property's value, use the property's label.

```c++
#include "json.hpp"

int main() {
    json::Json json("{ \"test\": 1, \"test2\": [ 0, 1, 2, 3 ] }");
    int testInt = json["test"];
}
```

To access an element of a list, use the index.
```c++
#include "json.hpp"

int main() {
    json::Json json("{ \"test\": 1, \"test2\": [ 0, 1, 2, 3 ] }");
    int testInt = json["test2"][0];
}
```

### Modifying

You can also use the `[]` operator to modify the JSON data.

```c++
#include "json.hpp"

int main() {
    json::Json json("{ \"test\": 1, \"test2\": [ 0, 1, 2, 3 ] }");
    json["test"] = 2;
}
```

### Getting the JSON

You can get formatted JSON back by using the `toString()` function.

```c++
#include "json.hpp"

#include <iostream>

int main() {
    json::Json json("{ \"test\": 1, \"test2\": [ 0, 1, 2, 3 ] }");
    json["test"] = 2;

    std::cout << json.toString() << std::endl;
}
```

### Types

JSON has several types.
- Boolean
- Null
- Number
- String
- Property
- Array
- Object

JSON objects (which are accessed with the `[]` operator) can be implicitly cast (as shown above), or they can be explicitly cast.

```c++
#include "json.hpp"

int main() {
    json::Json json("{ \"test\": 1, \"test2\": [ 0, 1, 2, 3 ] }");
    int testInt = json["test"].asInt();
}
```

To check a JSON object's type, you can use the following functions:
- `type()`: returns an enum
- `is(Type)`: returns true if the object is the provided type  
The enum value for each of the types is specified below.

#### Boolean

Type: `Type::Bool`

To cast a JSON `bool`, you can use the following functions:
- `asBool()`: returns a Boolean `true` or `false`
- `asInt()`: returns a `0` or `1`
- `asFloat()`: returns a `0.f` or `1.f`  
Any other casting will throw a runtime error.

#### Null

Type: `Type::Null`

I couldn't see a reason to implement getting a null value.

#### Int

Type: `Type::Int`

To cast a JSON `int`, you can use the following functions:
- `asInt()`: returns it as-is
- `asFloat()`: returns it as a `float`
- `asBool()`: returns it as `true` if it is not equal to `0`.  
Any other casting will throw a runtime error.

#### Float

Type: `Type::Float`

To cast a JSON `float`, you can use the following functions:
- `asInt()`: returns it truncated to an `int`
- `asFloat()`: returns it as-is
- `asBool()`: returns it as `true` if it is not equal to `0`.  
Any other casting will throw a runtime error.

#### String

Type: `Type::String`

To cast a JSON `string`, you can use the following functions:
- `asCString()`: returns a `const char *`
- `asStdString()`: returns a `std::string`  
Any other casting will throw a runtime error.

#### Property

Type: `Type::Property`

To cast a JSON `property`, you can use the following function:
- `asProperty()`: returns a `std::pair<std::string, JsonObject>` where the string is the property's name and the JsonObject is the property's value  
Any other casting will throw a runtime error.

#### Array

Type: `Type::Array`

To cast a JSON `array`, you can use the following function:
- `asArray()`: returns a `std::vector<JsonObject>`  
Any other casting will throw a runtime error.

#### Object

Type: `Type::Object`

To cast a JSON `object`, you can use the following function:
- `asObject()`: returns a `std::map<std::string, JsonObject>` where each key-value pair is a JSON property  
Any other casting will throw a runtime error.

## Errors

## 'to_string'/'atof'/'atoi' is not a member of 'std'

This is an issue I have run into when building for the ESP32. If you run into this error, I have implemented an easy fix. Open "stringpp.hpp" and uncomment the `#define` at the top.

```cpp
#pragma once

#include <sstream>
#include <string>

// Uncomment the line below if you get 'to_string'/'atof'/'atoi' is not a member
// of 'std'.
// clang-format off
#define _USE_STRINGPP
// clang-format on
```

This macro decides whether or not to use built-in C++ string functions or functions that I have made. The `stringpp` functions use `stringstream`s, which can be slow, so they are not enabled by default.
