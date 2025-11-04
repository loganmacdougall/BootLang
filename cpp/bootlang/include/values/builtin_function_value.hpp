#pragma once

#include <functional>
#include <sstream>
#include "value.hpp"

class BuiltinFunctionValue : public Value {
public:
  using Definition = std::function<Value::Ptr(Value::CallableInfo& info)>;

  Definition function;
  std::string doc;

  BuiltinFunctionValue(Definition function, std::string doc = "");
  
  virtual Value::Ptr call(Value::CallableInfo& info) override;
  virtual bool isCallable() const { return true; }
  virtual std::size_t hash() const override;
  virtual bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toString() const override;
};