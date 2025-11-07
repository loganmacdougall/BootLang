#pragma once

#include <functional>
#include "value.hpp"
#include "code_object.hpp"
#include "values/iterator_value.hpp"

class BuiltinGeneratorValue : public Value {
public:
  using InitialDefintion = std::function<std::shared_ptr<IteratorState>(const Value::CallableInfo&)>;
  using NextDefinition = std::function<Value::Ptr(std::shared_ptr<Value::IteratorState>)>;

  InitialDefintion init_function;
  NextDefinition next_function;
  const Value::CallableInfo info;

  BuiltinGeneratorValue(InitialDefintion init_function, NextDefinition next_function, Value::CallableInfo&& info);
  virtual Value::Ptr next(std::shared_ptr<Value::IteratorState> state) const override;
  virtual std::shared_ptr<Value::IteratorState> toIter() const override;
  virtual std::string toString() const override;
};