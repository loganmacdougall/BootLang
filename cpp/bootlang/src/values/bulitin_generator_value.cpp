#include "values/builtin_generator_value.hpp"

BuiltinGeneratorValue::BuiltinGeneratorValue(InitialDefintion init_function, NextDefinition next_function, Value::CallableInfo&& info)
: Value(Value::Type::BUILTIN_GENERATOR), init_function(std::move(init_function)),
  next_function(std::move(next_function)), info(std::move(info)) {}

Value::Ptr BuiltinGeneratorValue::next(std::shared_ptr<Value::IteratorState> state) const {
  return next_function(state);
}

std::shared_ptr<Value::IteratorState> BuiltinGeneratorValue::toIter() const {
  return init_function(info);
}

std::string BuiltinGeneratorValue::toString() const {
  std::stringstream out;
  out << "builtin generator <";
  out << static_cast<const void*>(this) << ">";
  return out.str();
}