#include <iostream>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <chrono>
#include "token.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "environment.hpp"
#include "compiler.hpp"
#include "virtual_machine.hpp"

std::string input_path = "../../tests/inputs/";
std::string expected_path = "../../tests/outputs/";

bool run_test(std::string filename);

int main() {
  bool all_passed = true;

  for (const auto &entry : std::filesystem::directory_iterator(input_path)) {
    std::string filename = entry.path().stem().string();
    std::cout << "Running test " << filename << "... ";

    auto start = std::chrono::high_resolution_clock::now();
    bool result = run_test(filename);
    auto end = std::chrono::high_resolution_clock::now();

    if (!result) {
      std::cout << std::endl << "test " << filename << " failed!" << std::endl;
      std::chrono::duration<double> elapsed = end - start;
      std::cout << std::fixed << std::setprecision(3)
        << "test " << filename << " failed!  "
        << elapsed.count() << "s" << std::endl;
      all_passed = false;
      break;
    }
  
    std::chrono::duration<double> elapsed = end - start;
    std::cout << std::fixed << std::setprecision(3)
      << "Passed!  " << elapsed.count() << "s" << std::endl;
  }

  if (all_passed) {
    std::cout << std::endl << "All tests pass!" << std::endl;
  }
    
  return 0;
}

bool run_test(std::string filename) {
  std::stringstream debug, out, expected, code;

  std::ifstream input_file(input_path + filename + ".bl");
  code << input_file.rdbuf();
  
  try {
    Tokenizer tokenizer(code.str());
    auto tokens = tokenizer.tokenize();
    
    size_t lineno = -1;
    for (size_t i = 0; i < tokens.size(); i++) {
      if (tokens.at(i).lineno != lineno) {
        lineno = tokens.at(i).lineno;
        if (i > 0) debug << std::endl;
        debug << lineno << ":\t ";
      }
      debug << TokenMetadata::GetInstance().GetTokenName(tokens.at(i).token) << " ";
    }
    debug << std::endl << std::endl;
    
    Parser parser(tokens);
    BlockNodePtr ast = parser.parse();
    
    debug << ast->toCode(0) << std::endl << std::endl;
    
    Program program = Compiler().compile(ast);
    
    debug << program.toDisassembly() << std::endl;

    Environment env;
    env.loadDefaults(out);

    VirtualMachine vm(env);
    vm.loadProgram(program);
    vm.setInstructionLimit(100000);
    vm.runProgram();
  } catch (std::exception &e) {
    std::cout << debug.str() << std::endl;
    std::cout << out.str() << std::endl;
    std::cout << e.what() << std::endl;
    return false;
  }

  std::ifstream expected_file(expected_path + filename + ".out");
  expected << expected_file.rdbuf();

  if (out.str() != expected.str()) {
    std::cout << std::endl << debug.str() << std::endl;
    std::cout << "Expected:" << std::endl << expected.str() << std::endl;
    std::cout << "Actual:" << std::endl << out.str() << std::endl;
    return false;
  }

  return true;
}