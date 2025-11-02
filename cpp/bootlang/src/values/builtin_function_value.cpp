#include "values/builtin_function_value.hpp"

BuiltinFunctionValue::BuiltinFunctionValue(Definition function, std::string doc)
: Value(Value::Type::BUILTIN_FUNCTION), function(function), doc(doc) {}

Value::Ptr BuiltinFunctionValue::call(const std::vector<Value::Ptr>& args) const {
  return function(args);
}

std::size_t BuiltinFunctionValue::hash() const {
  return reinterpret_cast<std::size_t>(function.target<void*>());
}

bool BuiltinFunctionValue::equal(const Value& other) const {
  return this == &other;
}

Value::Ptr BuiltinFunctionValue::clone() const {
  return std::make_shared<BuiltinFunctionValue>(function, doc);
}

std::string BuiltinFunctionValue::toString() const {
  std::stringstream out;
  out << "builtin function <";
  out << static_cast<const void*>(&function) << ">";
  return out.str();
}