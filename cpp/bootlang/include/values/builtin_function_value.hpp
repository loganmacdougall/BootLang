#pragma once

#include <functional>
#include <sstream>
#include "value.hpp"

class BuiltinFunctionValue : public Value {
public:
  using Definition = std::function<Value::Ptr(Value::Ptr& self, const std::vector<Value::Ptr>&)>;

  Definition function;
  std::string doc;

  BuiltinFunctionValue(Definition function, std::string doc = "");
  Value::Ptr call(Value::Ptr& self, const std::vector<Value::Ptr>& args) const;

  virtual std::size_t hash() const override;
  virtual bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toString() const override;
};