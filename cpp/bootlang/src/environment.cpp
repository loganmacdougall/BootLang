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
  auto MSBF = [](std::function<Value::Ptr(Value::CallableInfo&)> fn) {
    return std::make_shared<BuiltinFunctionValue>(std::move(fn));
  };

  builtins.emplace("print",
    MSBF(
      [&print_stream](Value::CallableInfo& info) -> Value::Ptr {
        print_stream << core_print(info.args);
        print_stream.flush();

        return NoneValue::NONE;
      }
  ));

  builtins.emplace("range", MSBF(core_range));
  builtins.emplace("len", MSBF(core_len));
  builtins.emplace("map", MSBF(core_map));
  builtins.emplace("sum", MSBF(core_sum));

  builtins.emplace("set", MSBF(to_set));
  builtins.emplace("str", MSBF(to_string));
  builtins.emplace("list", MSBF(to_list));
  builtins.emplace("tuple", MSBF(to_tuple));
  builtins.emplace("bool", MSBF(to_bool));

  dict_attributes.emplace("items", MSBF(dict_items));

  string_attributes.emplace("join", MSBF(string_join));
  string_attributes.emplace("lower", MSBF(string_lower));
  string_attributes.emplace("upper", MSBF(string_upper));
  string_attributes.emplace("title", MSBF(string_title));
}