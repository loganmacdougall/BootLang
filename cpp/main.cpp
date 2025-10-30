#include <iostream>
#include <stdexcept>
#include <fstream>
#include "token.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "environment.hpp"
#include "compiler.hpp"

std::string samples_path = "../../samples/";

int main() {
  std::ifstream sample_file(samples_path + "example07.bl");
  std::stringstream code_buffer;
  code_buffer << sample_file.rdbuf();
  
  try {
    Tokenizer tokenizer(code_buffer.str());
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
      std::cout << std::endl << std::endl;
    }
    
    Parser parser(tokens.value());
    BlockNodePtr ast = parser.parse();
    
    std::cout << ast->toCode(0) << std::endl << std::endl;
    
    Environment env;
    Compiler compiler(env);
    Program program = compiler.compile(ast);
    
    std::cout << program.toDisassembly() << std::endl;
  } catch (std::exception &e) {
    std::cout << std::endl << e.what() << std::endl;
  }
    
  return 0;
}