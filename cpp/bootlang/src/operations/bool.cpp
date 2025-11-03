#include "operations/bool.hpp"

Value::Ptr bool_or_bool(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<BoolValue> a = Value::toDerived<BoolValue>(a_base);
  std::shared_ptr<BoolValue> b = Value::toDerived<BoolValue>(b_base);
  return std::make_shared<BoolValue>(a->value || b->value);
}

Value::Ptr bool_and_bool(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<BoolValue> a = Value::toDerived<BoolValue>(a_base);
  std::shared_ptr<BoolValue> b = Value::toDerived<BoolValue>(b_base);
  return std::make_shared<BoolValue>(a->value && b->value);
}
