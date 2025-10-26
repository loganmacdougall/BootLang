#include "values/generator_value.hpp"

GeneratorValue::GeneratorValue(std::shared_ptr<FunctionValue> parent)
    : Value(Value::GENERATOR), parent(parent) {
    state.ip = 0;
    state.finished = false;
}