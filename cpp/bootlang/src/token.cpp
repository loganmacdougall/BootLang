#include "token.hpp"

TokenMetadata& TokenMetadata::GetInstance() {
  static TokenMetadata instance;
  return instance;
}

const std::string& TokenMetadata::GetTokenString(TokenType token) const {
  return token_strings[static_cast<size_t>(token)];
}

const std::string& TokenMetadata::GetTokenName(TokenType token) const {
  return token_names[static_cast<size_t>(token)];
}

TokenType TokenMetadata::GetTokenFromName(const std::string &name) const {
  auto it = token_name_map.find(name);
    if (it != token_name_map.end()) {
        return it->second;
    }
    return TokenType::t_INVALID_TOKEN;
}

const std::vector<RegexPair> &TokenMetadata::GetTokenRegexes() const {
  return token_regexes;
}
