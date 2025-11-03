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
  BuiltinFuncMap dict_attributes;
  BuiltinFuncMap set_attributes;
  BuiltinFuncMap tuple_attributes;
  const OpRegistry& opRegistry;

  Environment();
  std::shared_ptr<BuiltinFunctionValue> getAttribute(Value::Type type, std::string name) const;
  void loadDefaults(std::ostream& print_stream);
};