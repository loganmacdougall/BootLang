#include <iostream>
#include <stdexcept>
#include "token.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "environment.hpp"
#include "compiler.hpp"

std::string code = ""
"A = [1, 2, 3, 4]\n"
"a, b, c = A[1:]\n"
"print(\"a smaller than c\" if a < c else \"a greater or equal to c\")\n"
;

int main() {
  try {

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
      std::cout << std::endl;
    }
    
    Parser parser(tokens.value());
    BlockNodePtr ast = parser.parse();
    
    std::cout << ast->toCode(0) << std::endl;
    
    Environment env;
    Compiler compiler(env);
    Program program = compiler.compile(ast);
    
    std::cout << program.toDissassembly() << std::endl;
  } catch (std::exception &e) {
    std::cout << std::endl << e.what() << std::endl;
  }
    
  return 0;
}