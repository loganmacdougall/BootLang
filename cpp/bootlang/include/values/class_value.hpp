#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "values/function_value.hpp"

class ClassValue : public Value {
public:
  std::shared_ptr<FunctionValue> block;

  ClassValue(std::shared_ptr<FunctionValue> block);

  virtual Value::Ptr call(Value::CallableInfo& info) override;
  virtual std::size_t hash() const override;
  virtual bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toString() const override;
};