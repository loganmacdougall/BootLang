#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include "environment.hpp"
#include "toplevel_context.hpp"
#include "instruction.hpp"
#include "value.hpp"

class Program {
public:
  std::shared_ptr<TopLevelContext> context;
  std::shared_ptr<std::vector<std::shared_ptr<CodeObject>>> funcs;
  std::shared_ptr<std::unordered_map<size_t, size_t>> line_numbers;
  
  std::unordered_map<std::string, size_t> func_map;
  
  Program(const Program& other);
  Program& operator=(const Program& other);

  Program(std::shared_ptr<TopLevelContext> context,
  std::shared_ptr<std::vector<std::shared_ptr<CodeObject>>> funcs,
  std::shared_ptr<std::unordered_map<size_t, size_t>> line_numbers);
  std::string toDisassembly() const;

};