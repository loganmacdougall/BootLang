#include <iostream>
#include "token.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"

std::string code = ""
"a = 1\n"
"b = 2\n"
"c = a + b\n"
"(a, b, c) = (a+b, b+c, a+b+c)\n"
"c, b, a = a, b, 4\n"
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
  } else {
    size_t lineno = -1;
    for (size_t i = 0; i < tokens->size(); i++) {
      if (tokens->at(i).lineno != lineno) {
        lineno = tokens->at(i).lineno;
        if (i > 0) std::cout << std::endl;
        std::cout << lineno << ":\t ";
      }
      std::cout << TokenMetadata::GetInstance().GetTokenName(tokens->at(i).token) << " ";
    }
  }

  Parser parser(tokens.value());
  BlockNodePtr ast = parser.parse();

  std::cout << ast->toCode(0) << std::endl;

  return 0;
}