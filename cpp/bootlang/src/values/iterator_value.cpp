#include "values/iterator_value.hpp"

IterableValue::IterableValue(Value::Ptr parent, std::shared_ptr<IteratorState> state)
: Value(Value::Type::ITERATOR), parent(parent), state(std::move(state)) {}

Value::Ptr IterableValue::next() const {
  return parent->nextFromIter(state);
}

std::string IterableValue::toString() const {
  return "iter of " + parent->toString();
}