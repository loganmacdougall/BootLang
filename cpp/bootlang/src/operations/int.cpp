#include "operations/int.hpp"

Value::Ptr int_add_int(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  return std::make_shared<IntValue>(a->value + b->value);
}

Value::Ptr int_add_float(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  return std::make_shared<FloatValue>(static_cast<double>(a->value) + b->value);
}

Value::Ptr int_minus_int(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  return std::make_shared<IntValue>(a->value - b->value);
}

Value::Ptr int_minus_float(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  return std::make_shared<FloatValue>(static_cast<double>(a->value) - b->value);
}

Value::Ptr int_times_int(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  return std::make_shared<IntValue>(a->value * b->value);
}

Value::Ptr int_times_float(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  return std::make_shared<FloatValue>(static_cast<double>(a->value) * b->value);
}

Value::Ptr int_int_divide_int(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<IntValue>(a->value / b->value);
}

Value::Ptr int_int_divide_float(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / static_cast<long>(b->value));
}

Value::Ptr int_divide_int(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<IntValue>(a->value / b->value);
}

Value::Ptr int_divide_float(const Value& a_base, const Value& b_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(static_cast<double>(a->value) / b->value);
}

Value::Ptr int_to_float(const Value& a_base) {
  const IntValue* a = Value::toDerived<IntValue>(&a_base);
  return std::make_shared<FloatValue>(static_cast<double>(a->value));
}