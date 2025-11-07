#include "operations/set.hpp"

Value::Ptr to_set(Value::CallableInfo& info) {
  if (info.args.size() != 1) {
    throw std::runtime_error("Function expects one argument");
  }

  Value::Ptr value = info.args.back();

  if (value->type == Value::Type::SET) {
    return value;
  }

  auto iter = value->toIter();
  auto set_value = std::make_shared<SetValue>();

  while (!iter->finished) {
    set_value->storeKey(value->next(iter));
  }

  return set_value;
}