#pragma once

#include <unordered_map>
#include <tuple>
#include <functional>
#include <optional>
#include "values/all_values.hpp"
#include "token.hpp"
#include "operations/int.hpp"
#include "operations/float.hpp"

class BinaryOpRegistry {
public:
  typedef Value::Ptr (*BinaryOpFn)(Value::Ptr, Value::Ptr);

  BinaryOpRegistry();
  static const BinaryOpRegistry& GetInstance();
  std::optional<BinaryOpFn> get(Value::Type lhs, Value::Type rhs, Token::Type op) const;
  void reg(Value::Type lhs, Value::Type rhs, Token::Type op, BinaryOpFn fn);

  void throwNotFunction(Value::Type lhs, Value::Type rhs, Token::Type op) const;
private:
  
  static constexpr size_t hash(Value::Type lhs, Value::Type rhs, Token::Type op);
  std::unordered_map<size_t, BinaryOpFn> registry;

public:
  BinaryOpRegistry(BinaryOpRegistry const&) = delete;
  void operator=(BinaryOpRegistry const&) = delete;
};