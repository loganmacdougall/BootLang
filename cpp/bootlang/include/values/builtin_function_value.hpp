#pragma once

#include <functional>
#include <optional>
#include <sstream>
#include "value.hpp"

class BuiltinFunctionValue : public Value {
public:
  using Definition = std::function<Value::Ptr(Value::CallableInfo& info)>;

  Definition function;
  std::string doc;
  std::optional<Value::Type> self_type;
  long min_args, max_args;

  BuiltinFunctionValue(Definition function, std::string doc = "", std::optional<Value::Type> self_type = std::nullopt, long min_args = 0, long max_args = -1);
  
  virtual Value::Ptr call(Value::CallableInfo& info) override;
  virtual std::size_t hash() const override;
  virtual bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toString() const override;
};