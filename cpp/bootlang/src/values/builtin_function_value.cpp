#include "values/builtin_function_value.hpp"

BuiltinFunctionValue::BuiltinFunctionValue(Definition function, std::string doc, std::optional<Value::Type> self_type, long min_args, long max_args)
: Value(Value::Type::BUILTIN_FUNCTION), function(function), doc(doc), self_type(self_type), min_args(min_args), max_args(max_args) {}

Value::Ptr BuiltinFunctionValue::call(Value::CallableInfo& info) {
  return function(info);
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