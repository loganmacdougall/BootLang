#include "values/function_value.hpp"

  FunctionValue::FunctionValue(std::shared_ptr<CodeObject> code)
: Value(Value::Type::FUNCTION), code(code) {}

std::size_t FunctionValue::hash() const {
   return std::hash<std::shared_ptr<CodeObject>>()(code);
}

bool FunctionValue::equal(const Value& other) const {
  if (other.type != type) {
    return false;
  }

  const FunctionValue& other_func = static_cast<const FunctionValue&>(other);
  return code == other_func.code;
}


Value::Ptr FunctionValue::clone() const {
  return std::make_shared<FunctionValue>(FunctionValue(code));
}

std::string FunctionValue::toCode() const {
  std::stringstream out;
  out << "<code object - \"" << code->name << "\"";
  out << "(" << static_cast<const void*>(&code) << ")>";
  return out.str();
}