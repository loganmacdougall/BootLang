#include "values/function_value.hpp"

  FunctionValue::FunctionValue(const CodeObject& code)
: Value(Value::Type::FUNCTION), code(code) {}

Value FunctionValue::clone() const {
  return FunctionValue(code);
}