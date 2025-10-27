#include "binaryop_registry.hpp"

const BinaryOpRegistry& BinaryOpRegistry::GetInstance() {
  static BinaryOpRegistry instance;
  return instance;
}

std::optional<BinaryOpRegistry::BinaryOpFn> BinaryOpRegistry::get(Value::Type lhs, Value::Type rhs, Token::Type op) const {
  auto it = registry.find(hash(lhs, rhs, op));
  if (it == registry.end()) {
    return std::nullopt;
  }
  
  return std::optional<BinaryOpRegistry::BinaryOpFn>(it->second);
}

void BinaryOpRegistry::reg(Value::Type lhs, Value::Type rhs, Token::Type op, BinaryOpRegistry::BinaryOpFn fn) {
  size_t h = hash(lhs, rhs, op);
  registry[h] = fn;
}

constexpr size_t BinaryOpRegistry::hash(Value::Type lhs, Value::Type rhs, Token::Type op) {
  return (static_cast<size_t>(op))
    | (static_cast<size_t>(lhs) << 8)
    | (static_cast<size_t>(rhs) << 16);
}

BinaryOpRegistry::BinaryOpRegistry() {
  using V = Value::Type;
  using T = Token::Type;

  reg(V::INT, V::INT, T::PLUS, int_add_int);
  reg(V::INT, V::INT, T::PLUS_ASSIGN, int_add_int);
  reg(V::INT, V::INT, T::MINUS, int_minus_int);
  reg(V::INT, V::INT, T::MINUS_ASSIGN, int_minus_int);
  reg(V::INT, V::INT, T::STAR, int_times_int);
  reg(V::INT, V::INT, T::STAR_ASSIGN, int_times_int);
  reg(V::INT, V::INT, T::SLASH, int_divide_int);
  reg(V::INT, V::INT, T::SLASH_ASSIGN, int_divide_int);
  reg(V::INT, V::INT, T::DOUBLE_SLASH, int_int_divide_int);
  reg(V::INT, V::INT, T::DOUBLE_SLASH_ASSIGN, int_int_divide_int);

  reg(V::INT, V::FLOAT, T::PLUS, int_add_float);
  reg(V::INT, V::FLOAT, T::PLUS_ASSIGN, int_add_float);
  reg(V::INT, V::FLOAT, T::MINUS, int_minus_float);
  reg(V::INT, V::FLOAT, T::MINUS_ASSIGN, int_minus_float);
  reg(V::INT, V::FLOAT, T::STAR, int_times_float);
  reg(V::INT, V::FLOAT, T::STAR_ASSIGN, int_times_float);
  reg(V::INT, V::FLOAT, T::SLASH, int_divide_float);
  reg(V::INT, V::FLOAT, T::SLASH_ASSIGN, int_divide_float);
  reg(V::INT, V::FLOAT, T::DOUBLE_SLASH, int_int_divide_float);
  reg(V::INT, V::FLOAT, T::DOUBLE_SLASH_ASSIGN, int_int_divide_float);

  reg(V::FLOAT, V::INT, T::PLUS, float_add_int);
  reg(V::FLOAT, V::INT, T::PLUS_ASSIGN, float_add_int);
  reg(V::FLOAT, V::INT, T::MINUS, float_minus_int);
  reg(V::FLOAT, V::INT, T::MINUS_ASSIGN, float_minus_int);
  reg(V::FLOAT, V::INT, T::STAR, float_times_int);
  reg(V::FLOAT, V::INT, T::STAR_ASSIGN, float_times_int);
  reg(V::FLOAT, V::INT, T::SLASH, float_divide_int);
  reg(V::FLOAT, V::INT, T::SLASH_ASSIGN, float_divide_int);
  reg(V::FLOAT, V::INT, T::DOUBLE_SLASH, float_int_divide_int);
  reg(V::FLOAT, V::INT, T::DOUBLE_SLASH_ASSIGN, float_int_divide_int);

  reg(V::FLOAT, V::FLOAT, T::PLUS, float_add_float);
  reg(V::FLOAT, V::FLOAT, T::PLUS_ASSIGN, float_add_float);
  reg(V::FLOAT, V::FLOAT, T::MINUS, float_minus_float);
  reg(V::FLOAT, V::FLOAT, T::MINUS_ASSIGN, float_minus_float);
  reg(V::FLOAT, V::FLOAT, T::STAR, float_times_float);
  reg(V::FLOAT, V::FLOAT, T::STAR_ASSIGN, float_times_float);
  reg(V::FLOAT, V::FLOAT, T::SLASH, float_divide_float);
  reg(V::FLOAT, V::FLOAT, T::SLASH_ASSIGN, float_divide_float);
  reg(V::FLOAT, V::FLOAT, T::DOUBLE_SLASH, float_int_divide_float);
  reg(V::FLOAT, V::FLOAT, T::DOUBLE_SLASH_ASSIGN, float_int_divide_float);

}