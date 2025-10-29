#include "values/function_value.hpp"

  FunctionValue::FunctionValue(const CodeObject& code)
: Value(Value::Type::FUNCTION), code(code) {}

Value::Ptr FunctionValue::clone() const {
  return std::make_shared<FunctionValue>(FunctionValue(code));
}

std::string FunctionValue::toCode() const {
  std::stringstream out;
  out << "<code object - \"" << code.name << "\"";
  out << "(" << static_cast<const void*>(&code) << ")>";
  return out.str();
}