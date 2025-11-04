#include "operations/tuple.hpp"

Value::Ptr to_tuple(Value::CallableInfo& info) {
  if (info.args.size() != 1) {
    throw std::runtime_error("Function expects one argument");
  }

  if (!info.args.at(0)->isIterable()) {
    throw std::runtime_error("Expected iterable");
  }
  
  Value::Ptr iterable = info.args.at(0);

  if (iterable->type == Value::Type::LIST) {
    return iterable;
  }

  auto iter = iterable->iterInitialState();
  std::vector<Value::Ptr> elems;

  while (!iter->finished) {
    Value::Ptr value = iterable->nextFromIter(iter);
    elems.push_back(value);
  }

  return std::make_shared<TupleValue>(std::move(elems));
}