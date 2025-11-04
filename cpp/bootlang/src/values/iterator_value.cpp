#include "values/iterator_value.hpp"

IterableValue::IterableValue(Value::Ptr parent, std::shared_ptr<IteratorState> state)
: Value(Value::Type::ITERATOR), parent(parent), state(std::move(state)) {}

Value::Ptr IterableValue::nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const {
  return parent->nextFromIter(base_state);
}

std::shared_ptr<Value::IteratorState> IterableValue::iterInitialState() const {
  return state;
}


Value::Ptr IterableValue::next() const {
  return parent->nextFromIter(state);
}

std::string IterableValue::toString() const {
  return "iter of " + parent->toString();
}