#include <sstream>
#include <regex>
#include <iostream>
#include "tokenizer.hpp"

const uint32_t SPACES_IN_TABS = 2;

uint32_t count_indent(std::string &text) {
  uint32_t count = 0;
  for (char c : text) {
    if (c == ' ') count += 1;
    if (c == '\t') count += SPACES_IN_TABS;
  }

  return count;
}

std::string build_combined_pattern(std::vector<TokenMetadata::RegexPair> regex_pairs) {
  std::string regex;

  for (auto &regex_pair : regex_pairs) {
    if (regex.size() > 0) { regex += "|"; }
    regex += "(" + regex_pair.second + ")";
  }
  
  return regex;
}

Tokenizer::Tokenizer(std::string code) :
  code(std::move(code)) {}

  std::optional<std::vector<TokenData>> Tokenizer::tokenize() {
  std::vector<TokenData> tokens;
  std::vector<uint32_t> indent_stack{0};

  bool previous_was_newline = false;
  bool comment = false;
  uint32_t lineno = 1;
  uint32_t line_start = 0;
  
  static const auto regex_pairs = TokenMetadata::GetInstance().GetTokenRegexes();
  static const auto regex_pattern = build_combined_pattern(regex_pairs);  
  static const std::regex token_regex(build_combined_pattern(regex_pairs), std::regex::ECMAScript);

  auto begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
  auto end = std::sregex_iterator();

  for (auto it = begin; it != end; it++) {
    const std::smatch &match = *it;
    
    Token::Type token = Token::Type::INVALID_TOKEN;
    std::string text;

    for (size_t i = 1; i < match.size(); i++) {
      if (match[i].matched) {
        token = regex_pairs[i - 1].first;
        text = match[i].str();
        break;
      }
    }

    if (token == Token::Type::INVALID_TOKEN) {
      return std::nullopt;
    }

    uint32_t col = static_cast<uint32_t>(match.position() - line_start + 1);

    comment = token == Token::Type::COMMENT || comment;
    if (comment && token != Token::Type::NEWLINE) continue;
    comment = false;

    if (previous_was_newline && token != Token::Type::NEWLINE) {
      previous_was_newline = false;

      if (token != Token::Type::WHITESPACE) {
        while (indent_stack.size() > 1) {
          indent_stack.pop_back();
          tokens.push_back(TokenData{Token::Type::DEDENT, "", lineno, 1});
        }

        tokens.push_back(TokenData{token, text, lineno, col});
        continue;
      }

      uint32_t indent_count = count_indent(text);
      if (indent_count > indent_stack[indent_stack.size() - 1]) {
        indent_stack.push_back(indent_count);
        tokens.push_back(TokenData{Token::Type::INDENT, text, lineno, col});
        continue;
      } 
      
      while (indent_count < indent_stack[indent_stack.size() - 1]) {
        indent_stack.pop_back();
        tokens.push_back(TokenData{Token::Type::DEDENT, text, lineno, col});
      }

      if (indent_count != indent_stack[indent_stack.size() - 1]) {
        return std::nullopt;
      }

      continue;
    }

    if (token == Token::Type::NEWLINE) {
      previous_was_newline = true;
      lineno += 1;
      line_start = match.position() + match.length();
    }

    if (token == Token::Type::WHITESPACE)
      continue;

    tokens.push_back(TokenData{token, text, lineno, col});
  }

  while (indent_stack.size() > 1) {
    indent_stack.pop_back();
    tokens.push_back(TokenData{Token::Type::DEDENT, "", lineno, 1});
  }

  return tokens;
}