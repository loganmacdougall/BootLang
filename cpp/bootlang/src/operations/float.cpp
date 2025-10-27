#include "operations/float.hpp"

Value::Ptr float_add_float(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  return std::make_shared<FloatValue>(a->value + b->value);
}

Value::Ptr float_add_int(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  return std::make_shared<FloatValue>(a->value + static_cast<double>(b->value));
}

Value::Ptr float_minus_float(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  return std::make_shared<FloatValue>(a->value - b->value);
}

Value::Ptr float_minus_int(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  return std::make_shared<FloatValue>(a->value - static_cast<double>(b->value));
}

Value::Ptr float_times_float(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  return std::make_shared<FloatValue>(a->value * b->value);
}

Value::Ptr float_times_int(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  return std::make_shared<FloatValue>(a->value * static_cast<double>(b->value));
}

Value::Ptr float_divide_float(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / b->value);
}

Value::Ptr float_divide_int(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / static_cast<double>(b->value));
}

Value::Ptr float_int_divide_float(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const FloatValue* b = Value::toDerived<FloatValue>(&b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / static_cast<long>(b->value));
}

Value::Ptr float_int_divide_int(const Value& a_base, const Value& b_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  const IntValue* b = Value::toDerived<IntValue>(&b_base);
  if (b->value == 0) {
    throw std::runtime_error("Division by zero");
  }
  return std::make_shared<FloatValue>(a->value / static_cast<double>(b->value));
}

Value::Ptr float_to_int(const Value& a_base) {
  const FloatValue* a = Value::toDerived<FloatValue>(&a_base);
  return std::make_shared<IntValue>(static_cast<long>(a->value));
}