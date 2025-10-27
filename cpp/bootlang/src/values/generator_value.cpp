#include "values/generator_value.hpp"

GeneratorValue::GeneratorValue(const CodeObject& code)
: Value(Value::GENERATOR), code(code) {}

Value GeneratorValue::clone() const {
  return GeneratorValue(code);
}