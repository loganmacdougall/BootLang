#pragma once

#include "value.hpp"
#include "values/function_value.hpp"

struct GeneratorState {
  size_t ip;
  std::vector<Value::Ptr> stack;
  std::vector<Value::Ptr> locals;
  bool finished;
};

class GeneratorValue : public Value {
  public:
    std::shared_ptr<FunctionValue> parent;
    GeneratorState state;

    GeneratorValue(std::shared_ptr<FunctionValue> parent);
};