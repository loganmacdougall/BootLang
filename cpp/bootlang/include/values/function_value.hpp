#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "value.hpp"
#include "instruction.hpp"

class FunctionValue : public Value {
  public:
    std::string name;
    std::vector<std::string> parameters;
    std::vector<Instruction> instructions;
    std::vector<Value::Ptr> constants;
    std::unordered_map<std::string, size_t> vars;
    std::shared_ptr<FunctionValue> parent_function;

    FunctionValue(const std::string&& name,
                  const std::vector<std::string>&& parameters,
                  const std::vector<Instruction>&& instructions,
                  const std::vector<Value::Ptr>&& constants,
                  const std::unordered_map<std::string, size_t>&& vars,
                  std::shared_ptr<FunctionValue> parent_function = nullptr);
};
