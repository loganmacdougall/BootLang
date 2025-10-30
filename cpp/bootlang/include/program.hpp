#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include "environment.hpp"
#include "toplevel_context.hpp"
#include "instruction.hpp"
#include "value.hpp"

class Program {
  const std::shared_ptr<TopLevelContext> context;
  const Environment& env;
  
public:
  Program(std::shared_ptr<TopLevelContext> context, const Environment& env);
  std::string toDisassembly() const;
};