#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "code_object.hpp"
#include "value.hpp"
#include "instruction.hpp"

class FunctionValue : public Value {
public:
  const CodeObject& code;

  FunctionValue(const CodeObject& code);
  virtual Value::Ptr clone() const override;
  virtual std::string toCode() const override;
};