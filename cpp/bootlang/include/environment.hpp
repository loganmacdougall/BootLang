#pragma once

#include <unordered_map>
#include <op_registry.hpp>
#include <ostream>
#include "value.hpp"
#include "operations/all_operations.hpp"
#include "values/function_value.hpp"

typedef std::unordered_map<std::string, std::shared_ptr<BuiltinFunctionValue>> BuiltinFuncMap;

struct Environment {
  BuiltinFuncMap builtins;
  BuiltinFuncMap int_attributes;
  BuiltinFuncMap float_attributes;
  BuiltinFuncMap string_attributes;
  BuiltinFuncMap list_attributes;
  BuiltinFuncMap map_attributes;
  BuiltinFuncMap set_attributes;
  BuiltinFuncMap tuple_attributes;
  const OpRegistry& opRegistry;

  Environment();
  void loadDefaults(std::ostream& print_stream);
};