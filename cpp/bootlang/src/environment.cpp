#include "environment.hpp"

Environment::Environment()
: opRegistry(OpRegistry::GetInstance()) {}

void Environment::loadDefaults(std::ostream& print_stream) {
  builtins.emplace("print",
    std::make_shared<BuiltinFunctionValue>(
      [&print_stream](const std::vector<Value::Ptr>& args) mutable -> Value::Ptr {
        print_stream << core_print(args);
        print_stream.flush();

        return NoneValue::NONE;
      }
  ));

  builtins.emplace("range", std::make_shared<BuiltinFunctionValue>(core_range));
}