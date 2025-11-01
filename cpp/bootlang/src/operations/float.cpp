#include "operations/float.hpp"

Value::Ptr float_add_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  return std::make_shared<FloatValue>(a->value + b->value);
}

Value::Ptr float_add_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  return std::make_shared<FloatValue>(a->value + static_cast<double>(b->value));
}

Value::Ptr float_minus_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  return std::make_shared<FloatValue>(a->value - b->value);
}

Value::Ptr float_minus_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  return std::make_shared<FloatValue>(a->value - static_cast<double>(b->value));
}

Value::Ptr float_times_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  return std::make_shared<FloatValue>(a->value * b->value);
}

Value::Ptr float_times_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  return std::make_shared<FloatValue>(a->value * static_cast<double>(b->value));
}

Value::Ptr float_divide_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / b->value);
}

Value::Ptr float_divide_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / static_cast<double>(b->value));
}

Value::Ptr float_int_divide_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / static_cast<long>(b->value));
}

Value::Ptr float_int_divide_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / static_cast<double>(b->value));
}

Value::Ptr float_to_int(Value::Ptr a_base) {
  std::shared_ptr<FloatValue> a = Value::toDerived<FloatValue>(a_base);
  return std::make_shared<IntValue>(static_cast<long>(a->value));
}