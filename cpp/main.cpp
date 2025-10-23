#include <iostream>
#include "token.hpp"
#include "tokenizer.hpp"

std::string code = ""
"a = 1\n"
"b = 2\n"
"c = a + b\n"
"\n"
"print(a, b, c)\n"
"\n"
"if c == 3:\n"
"\tprint(\"c is 3\")\n"
"else:\n"
"\tprint(\"c is not 3\")\n";

int main() {
  Tokenizer tokenizer(code);
  auto tokens = tokenizer.tokenize();

  if (!tokens) {
    std::cout << "Syntax Error when tokenizing the code" << std::endl;
    return 0;
  }

  std::cout << "-- TOKENS START --" << std::endl;
  for (const auto &token_data : tokens.value()) {
    std::string token_name = TokenMetadata::GetInstance().GetTokenName(token_data.token);
    std::cout << "[" << token_name << "] ";
  }
  std::cout << std::endl << "--  TOKENS END  --" << std::endl;

  return 0;
}