#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "code_object.hpp"
#include "value.hpp"
#include "instruction.hpp"

class FunctionValue : public Value {
public:
  const std::shared_ptr<CodeObject> code;
  std::vector<Value::Ptr> freevars;

  FunctionValue(std::shared_ptr<CodeObject> code);
  void setFreeVars(const std::vector<Value::Ptr>& freevars);

  virtual Value::Ptr call(Value::CallableInfo& info) override;
  virtual std::size_t hash() const override;
  virtual bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toString() const override;
};