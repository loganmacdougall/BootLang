#include "operations/tuple.hpp"

Value::Ptr to_tuple(Value::CallableInfo& info) {
  Value::Ptr iterable = info.args.at(0);

  if (iterable->type == Value::Type::LIST) {
    return iterable;
  }

  auto iter = iterable->toIter();
  std::vector<Value::Ptr> elems;

  while (!iter->finished) {
    Value::Ptr value = iterable->next(iter);
    elems.push_back(value);
  }

  return std::make_shared<TupleValue>(std::move(elems));
}