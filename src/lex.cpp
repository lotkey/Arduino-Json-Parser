#include "json.hpp"

#include <algorithm>

namespace json {
size_t Json::find_first_of_any(const std::string &text,
                               const std::vector<char> &delimiters) {
  for (int i = 0; i < text.length(); i++) {
    if (std::find(delimiters.begin(), delimiters.end(), text[i]) !=
        delimiters.end()) {
      return i;
    }
  }
  return std::string::npos;
}

std::vector<std::string> Json::split_into_tokens(const std::string &json) {
  // First, separate strings from non-strings
  auto tokens1 = split_by_string_literals(json);

  // Second, split everything except for the string literals by whitespace
  std::vector<std::string> tokens2;
  for (const auto &word : tokens1) {
    if (word[0] == '"') {
      tokens2.push_back(word);
    } else {
      auto word_splits = split(word);
      for (const auto &token : word_splits) {
        tokens2.push_back(token);
      }
    }
  }

  // Next, split everything except for the string literals into tokens
  std::vector<std::string> tokens3;
  std::vector<char> terminals = {'{', '}', '[', ']', ',', ':'};
  for (const auto &word : tokens2) {
    if (word[0] == '"') {
      tokens3.push_back(word);
    } else {
      auto word_splits = split(word, terminals, true);
      for (const auto &token : word_splits) {
        tokens3.push_back(token);
      }
    }
  }

  return tokens3;
}

std::vector<std::string>
Json::split_by_string_literals(const std::string &str) {
  std::vector<std::string> words;
  int word_begin = 0;
  bool inString = false;

  for (int i = 0; i < str.length(); i++) {
    if (inString && str[i] == '\\') {
      i++;
    } else if (str[i] == '"') {
      if (inString) {
        std::string word = str.substr(word_begin, (i - word_begin) + 1);
        words.push_back(word);
        inString = false;
        word_begin = i + 1;
      } else {
        std::string word = str.substr(word_begin, i - word_begin);
        words.push_back(word);
        inString = true;
        word_begin = i;
      }
    }
  }

  if (inString) {
    throw std::runtime_error("Lexing error: Unclosed string: \"" + str + "\".");
  }

  std::string last_word =
      str.substr(word_begin, (str.length() - word_begin) + 1);
  words.push_back(last_word);
  return words;
}

std::vector<std::string> Json::split(std::string str,
                                     const std::vector<char> &separators,
                                     bool includeSeparators) {
  std::vector<std::string> words;

  for (size_t pos = find_first_of_any(str, separators);
       pos != std::string::npos; pos = find_first_of_any(str, separators)) {
    std::string word = str.substr(0, pos);
    if (!word.empty()) {
      words.push_back(word);
    }

    if (includeSeparators) {
      words.push_back(str.substr(pos, 1));
    }

    str.erase(0, pos + 1);
  }

  if (!str.empty()) {
    words.push_back(str);
  }

  return words;
}
} // namespace json