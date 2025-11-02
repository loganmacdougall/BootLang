#pragma once

#include <functional>
#include "value.hpp"
#include "code_object.hpp"
#include "values/iterator_value.hpp"

class BuiltinGeneratorValue : public Value {
public:
  using InitialDefintion = std::function<std::shared_ptr<IteratorState>(const std::vector<Value::Ptr>&)>;
  using NextDefinition = std::function<Value::Ptr(std::shared_ptr<Value::IteratorState>)>;

  InitialDefintion init_function;
  NextDefinition next_function;
  std::vector<Value::Ptr> args;

  BuiltinGeneratorValue(InitialDefintion init_function, NextDefinition next_function, std::vector<Value::Ptr> args);
  virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> state) const override;
  virtual std::shared_ptr<Value::IteratorState> iterInitialState() const override;
  bool isIterable() const override { return true; }
  virtual std::string toString() const override;
};