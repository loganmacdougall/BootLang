#include "values/function_value.hpp"

FunctionValue::FunctionValue(const std::string&& name,
                                 const std::vector<std::string>&& parameters,
                                 const std::vector<Instruction>&& instructions,
                                 const std::vector<Value::Ptr>&& constants,
                                 const std::unordered_map<std::string, size_t>&& vars,
                                 std::shared_ptr<FunctionValue> parent_function)
    : Value(Value::Type::FUNCTION),
      name(std::move(name)),
      parameters(std::move(parameters)),
      instructions(std::move(instructions)),
      constants(std::move(constants)),
      vars(std::move(vars)),
      parent_function(std::move(parent_function)) {}