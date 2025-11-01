#include "operations/int.hpp"

Value::Ptr int_add_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  return std::make_shared<IntValue>(a->value + b->value);
}

Value::Ptr int_add_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  return std::make_shared<FloatValue>(static_cast<double>(a->value) + b->value);
}

Value::Ptr int_minus_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  return std::make_shared<IntValue>(a->value - b->value);
}

Value::Ptr int_minus_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  return std::make_shared<FloatValue>(static_cast<double>(a->value) - b->value);
}

Value::Ptr int_times_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  return std::make_shared<IntValue>(a->value * b->value);
}

Value::Ptr int_times_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  return std::make_shared<FloatValue>(static_cast<double>(a->value) * b->value);
}

Value::Ptr int_int_divide_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<IntValue>(a->value / b->value);
}

Value::Ptr int_int_divide_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / static_cast<long>(b->value));
}

Value::Ptr int_divide_int(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<IntValue>(a->value / b->value);
}

Value::Ptr int_divide_float(Value::Ptr a_base, Value::Ptr b_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  std::shared_ptr<FloatValue> b = Value::toDerived<FloatValue>(b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(static_cast<double>(a->value) / b->value);
}

Value::Ptr int_to_float(Value::Ptr a_base) {
  std::shared_ptr<IntValue> a = Value::toDerived<IntValue>(a_base);
  return std::make_shared<FloatValue>(static_cast<double>(a->value));
}