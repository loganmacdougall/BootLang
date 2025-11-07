#include "values/iterator_value.hpp"

IterableValue::IterableValue(Value::Ptr parent, std::shared_ptr<IteratorState> state)
: Value(Value::Type::ITERATOR), parent(parent), state(std::move(state)) {}

Value::Ptr IterableValue::next(std::shared_ptr<Value::IteratorState> base_state) const {
  return parent->next(base_state);
}

std::shared_ptr<Value::IteratorState> IterableValue::toIter() const {
  return state;
}


Value::Ptr IterableValue::next() const {
  return parent->next(state);
}

std::string IterableValue::toString() const {
  return "iter of " + parent->toString();
}