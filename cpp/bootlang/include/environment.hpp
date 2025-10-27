#pragma once

#include <unordered_map>
#include <binaryop_registry.hpp>
#include "value.hpp"
#include "values/function_value.hpp"

typedef std::unordered_map<std::string, std::shared_ptr<FunctionValue>> References;

struct Environment {
  References builtins;
  References int_attributes;
  References float_attributes;
  References string_attributes;
  References list_attributes;
  References map_attributes;
  References set_attributes;
  References tuple_attributes;
  const BinaryOpRegistry& binaryOpRegistry;

  Environment();
};