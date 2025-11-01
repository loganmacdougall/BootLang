#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include "token.hpp"

struct TokenData {
    Token::Type token;
    std::string text;
    size_t lineno;
    size_t col;
};

class Tokenizer {
  const std::string code;

  public:
    explicit Tokenizer(std::string code);

    std::optional<std::vector<TokenData>> tokenize();
};