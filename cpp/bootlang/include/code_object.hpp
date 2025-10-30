#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include "function_context.hpp"
#include "instruction.hpp"
#include "value.hpp"

class CodeObject {
public:  
  std::string name;
  std::string doc = "";
  std::vector<std::string> parameters;
  std::shared_ptr<FunctionContext> context;

  CodeObject(const std::string& name,
                const std::string& doc,
                const std::vector<std::string>&& parameters,
                std::shared_ptr<FunctionContext> context);

  std::string toDisassembly() const;
};