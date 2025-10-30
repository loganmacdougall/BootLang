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

  FunctionValue(std::shared_ptr<CodeObject> code);
  virtual std::size_t hash() const override;
  virtual bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toCode() const override;
};