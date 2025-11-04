#include "values/function_value.hpp"

FunctionValue::FunctionValue(std::shared_ptr<CodeObject> code)
: Value(Value::Type::FUNCTION), code(code) {}

void FunctionValue::setFreeVars(const std::vector<Value::Ptr>& freevars) {
  this->freevars = freevars;
}

Value::Ptr FunctionValue::call(Value::CallableInfo& info) {
  return info.vm_call(info.func_pointer, info.self, std::move(info.args));
}

std::size_t FunctionValue::hash() const {
   return std::hash<std::shared_ptr<CodeObject>>()(code);
}

bool FunctionValue::equal(const Value& other) const {
  if (other.type != type) {
    return false;
  }

  const FunctionValue& other_func = static_cast<const FunctionValue&>(other);
  return code == other_func.code;
}


Value::Ptr FunctionValue::clone() const {
  auto other = std::make_shared<FunctionValue>(FunctionValue(code));
  other->freevars = freevars;
  return other;
}

std::string FunctionValue::toString() const {
  std::stringstream out;
  out << "code object <\"" << code->name << "\": ";
  out << static_cast<const void*>(code.get()) << ">";
  return out.str();
}