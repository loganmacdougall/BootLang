#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include "token.hpp"

struct TokenData {
    BLToken token;
    std::string text;
    uint32_t lineno;
    uint32_t col;
};

class Tokenizer {
  const std::string code;

  public:
    explicit Tokenizer(std::string code);

    std::optional<std::vector<TokenData>> tokenize();
};