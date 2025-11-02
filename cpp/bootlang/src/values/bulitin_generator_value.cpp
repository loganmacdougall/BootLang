#include "values/builtin_generator_value.hpp"

BuiltinGeneratorValue::BuiltinGeneratorValue(InitialDefintion init_function, NextDefinition next_function, std::vector<Value::Ptr> args)
: Value(Value::Type::BUILTIN_GENERATOR), init_function(init_function),
  next_function(next_function), args(args) {}

Value::Ptr BuiltinGeneratorValue::nextFromIter(std::shared_ptr<Value::IteratorState> state) const {
  return next_function(state);
}

std::shared_ptr<Value::IteratorState> BuiltinGeneratorValue::iterInitialState() const {
  return init_function(args);
}

std::string BuiltinGeneratorValue::toString() const {
  std::stringstream out;
  out << "builtin generator <";
  out << static_cast<const void*>(this) << ">";
  return out.str();
}