#pragma once

#include <unordered_map>
#include <tuple>
#include <functional>
#include <optional>
#include "values/all_values.hpp"
#include "token.hpp"
#include "operations/int.hpp"
#include "operations/float.hpp"

class OpRegistry {
public:
  typedef Value::Ptr (*BinaryOpFn)(Value::Ptr, Value::Ptr);
  typedef Value::Ptr (*UnaryOpFn)(Value::Ptr);

  OpRegistry();
  static const OpRegistry& GetInstance();
  std::optional<BinaryOpFn> get(Value::Type lhs, Value::Type rhs, Token::Type op) const;
  std::optional<UnaryOpFn>  get(Value::Type rhs, Token::Type op) const;
  void reg(Value::Type lhs, Value::Type rhs, Token::Type op, BinaryOpFn fn);
  void reg(Value::Type rhs, Token::Type op, UnaryOpFn fn);

  void throwNotFunction(Value::Type lhs, Value::Type rhs, Token::Type op) const;
  void throwNotFunction(Value::Type rhs, Token::Type op) const;
private:
  
  static constexpr size_t hash(Value::Type lhs, Value::Type rhs, Token::Type op);
  static constexpr size_t hash(Value::Type rhs, Token::Type op);
  std::unordered_map<size_t, BinaryOpFn> binary_registry;
  std::unordered_map<size_t, UnaryOpFn> unary_registry;

public:
  OpRegistry(OpRegistry const&) = delete;
  void operator=(OpRegistry const&) = delete;
};