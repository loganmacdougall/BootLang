#include "value.hpp"

Value::CallableInfo::CallableInfo(
    Value::Ptr func_pointer,
    Value::Ptr self,
    std::vector<Value::Ptr>&& args,
    VMCallback vm_call
) : func_pointer(func_pointer), self(self), args(std::move(args)), vm_call(vm_call) {}

Value::Value(Type type) : type(type) {}

Value::Ptr Value::call(CallableInfo& info) {
    (void)info;
    const std::string& ts = typeString();
    throw std::runtime_error("call not implemented for type \"" + ts + "\"");
}

size_t Value::hash() const {
    const std::string& ts = typeString();
    throw std::runtime_error("hash not implemented for type \"" + ts + "\"");
}

bool Value::equal(const Value& other) const {
    (void)other;
    const std::string& ts = typeString();
    throw std::runtime_error("hash implemented for type \"" + ts + "\"");
}

Value::Ptr Value::next(std::shared_ptr<Value::IteratorState> state) const {
    (void)state;
    const std::string& ts = typeString();
    throw std::runtime_error("next not implemented for type \"" + ts + "\"");
}

std::shared_ptr<Value::IteratorState> Value::toIter() const {
    const std::string& ts = typeString();
    throw std::runtime_error("iter not implemented for type \"" + ts + "\"");
}

size_t Value::len() const {
    const std::string& ts = typeString();
    throw std::runtime_error("len not implemented for type \"" + ts + "\"");
};

Value::Ptr Value::clone() const {
    const std::string& ts = typeString();
    throw std::runtime_error("clone not implemented for type \"" + ts + "\"");
}

std::string Value::toString() const {
    const std::string& ts = typeString();
    throw std::runtime_error("str not implemented for type \"" + ts + "\"");
}

const std::string& Value::typeString() const {
    return ValueMetadata::GetInstance().GetValueName(type);
}

const ValueMetadata& ValueMetadata::GetInstance() {
  static ValueMetadata instance;
  return instance;
}

const std::string& ValueMetadata::GetValueName(Value::Type value) const {
  return value_names[static_cast<size_t>(value)];
}