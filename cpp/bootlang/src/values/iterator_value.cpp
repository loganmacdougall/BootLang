#include "values/iterator_value.hpp"

IterableValue::IterableValue(Value::Ptr parent, Value::IteratorState&& state)
: Value(Value::Type::ITERATOR), parent(parent), state(std::move(state)) {}