#include "values/generator_value.hpp"

GeneratorValue::GeneratorValue(const CodeObject& code)
: Value(Value::GENERATOR), code(code) {}

Value::Ptr GeneratorValue::clone() const {
  return std::make_shared<GeneratorValue>(GeneratorValue(code));
}

std::string GeneratorValue::toCode() const {
  std::stringstream out;
  out << "<code object - \"" << code.name << "\"";
  out << "(" << static_cast<const void*>(&code) << ")>";
  return out.str();
}