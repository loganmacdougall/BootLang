#include "environment.hpp"

Environment::Environment()
: opRegistry(OpRegistry::GetInstance()) {}

std::shared_ptr<BuiltinFunctionValue> Environment::getAttribute(Value::Type type, std::string name) const {
  const BuiltinFuncMap* func_map;
  switch (type) {
    case Value::Type::INT:
        func_map = &int_attributes;
        break;
    case Value::Type::FLOAT:
        func_map = &float_attributes;
        break;
    case Value::Type::STRING:
        func_map = &string_attributes;
        break;
    case Value::Type::LIST:
        func_map = &list_attributes;
        break;
    case Value::Type::DICT:
        func_map = &dict_attributes;
        break;
    case Value::Type::SET:
        func_map = &set_attributes;
        break;
    case Value::Type::TUPLE:
        func_map = &tuple_attributes;
        break;
    default:
        throw std::runtime_error("Attribute lookup not supported for type");
  }

  auto it = func_map->find(name);
  if (it == func_map->end()) return nullptr;
  return it->second;
}

void Environment::loadDefaults(std::ostream& print_stream) {
  builtins.emplace("print",
    std::make_shared<BuiltinFunctionValue>(
      [&print_stream](Value::Ptr&, const std::vector<Value::Ptr>& args) mutable -> Value::Ptr {
        print_stream << core_print(args);
        print_stream.flush();

        return NoneValue::NONE;
      }
  ));

  builtins.emplace("range", std::make_shared<BuiltinFunctionValue>(core_range));
  builtins.emplace("len", std::make_shared<BuiltinFunctionValue>(core_len));

  dict_attributes.emplace("items", std::make_shared<BuiltinFunctionValue>(dict_items));
}