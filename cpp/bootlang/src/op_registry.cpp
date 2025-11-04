#include "op_registry.hpp"

const OpRegistry& OpRegistry::GetInstance() {
  static OpRegistry instance;
  return instance;
}

std::optional<OpRegistry::BinaryOpFn> OpRegistry::get(Value::Type lhs, Value::Type rhs, Token::Type op) const {
  auto it = binary_registry.find(hash(lhs, rhs, op));
  if (it == binary_registry.end()) {
    return std::nullopt;
  }
  
  return std::optional<OpRegistry::BinaryOpFn>(it->second);
}

std::optional<OpRegistry::UnaryOpFn> OpRegistry::get(Value::Type rhs, Token::Type op) const {
  auto it = unary_registry.find(hash(rhs, op));
  if (it == unary_registry.end()) {
    return std::nullopt;
  }
  
  return std::optional<OpRegistry::UnaryOpFn>(it->second);
}

void OpRegistry::reg(Value::Type lhs, Value::Type rhs, Token::Type op, OpRegistry::BinaryOpFn fn) {
  size_t h = hash(lhs, rhs, op);
  binary_registry[h] = fn;
}

void OpRegistry::reg(Value::Type rhs, Token::Type op, OpRegistry::UnaryOpFn fn) {
  size_t h = hash(rhs, op);
  unary_registry[h] = fn;
}

void OpRegistry::throwNotFunction(Value::Type lhs, Value::Type rhs, Token::Type op) const {
  std::ostringstream out;

  const std::string& lhs_str = ValueMetadata::GetInstance().GetValueName(lhs);
  const std::string& rhs_str = ValueMetadata::GetInstance().GetValueName(rhs);
  const std::string& op_str = Token::typeString(op);

  out << "No support for binary operator ";
  out << lhs_str << " " << op_str << " " << rhs_str;

  throw std::runtime_error(out.str());
}

void OpRegistry::throwNotFunction(Value::Type rhs, Token::Type op) const {
  std::ostringstream out;

  const std::string& rhs_str = ValueMetadata::GetInstance().GetValueName(rhs);
  const std::string& op_str = Token::typeString(op);

  out << "No support for unary operator ";
  out << op_str << " " << rhs_str;

  throw std::runtime_error(out.str());
}

constexpr size_t OpRegistry::hash(Value::Type lhs, Value::Type rhs, Token::Type op) {
  return (static_cast<size_t>(op))
    | (static_cast<size_t>(lhs) << 8)
    | (static_cast<size_t>(rhs) << 16);
}

constexpr size_t OpRegistry::hash(Value::Type rhs, Token::Type op) {
  return (static_cast<size_t>(op))
    | (static_cast<size_t>(rhs) << 8);
}

OpRegistry::OpRegistry() {
  using V = Value::Type;
  using T = Token::Type;

  reg(V::INT, T::MINUS, int_negitive);
  reg(V::FLOAT, T::MINUS, float_negitive);

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
  reg(V::INT, V::INT, T::PERCENT, int_mod_int);
  reg(V::INT, V::INT, T::PERCENT_ASSIGN, int_mod_int);
  reg(V::INT, V::INT, T::GREATER_EQUAL, int_ge_int);
  reg(V::INT, V::INT, T::GREATER, int_g_int);
  reg(V::INT, V::INT, T::LESS_EQUAL, int_le_int);
  reg(V::INT, V::INT, T::LESS, int_l_int);
  reg(V::INT, V::INT, T::EQUAL, int_eq_int);
  reg(V::INT, V::INT, T::NEQUAL, int_neq_int);

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
  reg(V::INT, V::FLOAT, T::PERCENT, int_mod_float);
  reg(V::INT, V::FLOAT, T::PERCENT_ASSIGN, int_mod_float);
  reg(V::INT, V::FLOAT, T::GREATER_EQUAL, int_ge_float);
  reg(V::INT, V::FLOAT, T::GREATER, int_g_float);
  reg(V::INT, V::FLOAT, T::LESS_EQUAL, int_le_float);
  reg(V::INT, V::FLOAT, T::LESS, int_l_float);
  reg(V::INT, V::FLOAT, T::EQUAL, int_eq_float);
  reg(V::INT, V::FLOAT, T::NEQUAL, int_neq_float);

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
  reg(V::FLOAT, V::INT, T::PERCENT, float_mod_int);
  reg(V::FLOAT, V::INT, T::PERCENT_ASSIGN, float_mod_int);
  reg(V::FLOAT, V::INT, T::GREATER_EQUAL, float_ge_int);
  reg(V::FLOAT, V::INT, T::GREATER, float_g_int);
  reg(V::FLOAT, V::INT, T::LESS_EQUAL, float_le_int);
  reg(V::FLOAT, V::INT, T::LESS, float_l_int);
  reg(V::FLOAT, V::INT, T::EQUAL, float_eq_int);
  reg(V::FLOAT, V::INT, T::NEQUAL, float_neq_int);

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
  reg(V::FLOAT, V::FLOAT, T::PERCENT, float_mod_float);
  reg(V::FLOAT, V::FLOAT, T::PERCENT_ASSIGN, float_mod_float);
  reg(V::FLOAT, V::FLOAT, T::GREATER_EQUAL, float_ge_float);
  reg(V::FLOAT, V::FLOAT, T::GREATER, float_g_float);
  reg(V::FLOAT, V::FLOAT, T::LESS_EQUAL, float_le_float);
  reg(V::FLOAT, V::FLOAT, T::LESS, float_l_float);
  reg(V::FLOAT, V::FLOAT, T::EQUAL, float_eq_float);
  reg(V::FLOAT, V::FLOAT, T::NEQUAL, float_neq_float);

  reg(V::STRING, V::STRING, T::EQUAL, string_eq_string);
  reg(V::STRING, V::STRING, T::PLUS, string_add_string);
  reg(V::STRING, V::STRING, T::PLUS_ASSIGN, string_add_string);

  reg(V::BOOL, V::BOOL, T::OR, bool_or_bool);
  reg(V::BOOL, V::BOOL, T::AND, bool_and_bool);

  reg(V::NONE, T::NOT, not_bool);
  reg(V::INT, T::NOT, not_bool);
  reg(V::FLOAT, T::NOT, not_bool);
  reg(V::CHAR, T::NOT, not_bool);
  reg(V::STRING, T::NOT, not_bool);
  reg(V::BOOL, T::NOT, not_bool);
  reg(V::FUNCTION, T::NOT, not_bool);
  reg(V::BUILTIN_FUNCTION, T::NOT, not_bool);
  reg(V::BUILTIN_GENERATOR, T::NOT, not_bool);
  reg(V::LIST, T::NOT, not_bool);
  reg(V::SLICE, T::NOT, not_bool);
  reg(V::DICT, T::NOT, not_bool);
  reg(V::SET, T::NOT, not_bool);
  reg(V::TUPLE, T::NOT, not_bool);
  reg(V::GENERATOR, T::NOT, not_bool);
  reg(V::ITERATOR, T::NOT, not_bool);

}