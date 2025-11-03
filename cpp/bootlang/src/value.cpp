#include "value.hpp"

Value::Value(Type type) : type(type) {}


size_t Value::hash() const {
    throw std::runtime_error("hash not implemented for this Value type");
}

bool Value::equal(const Value& other) const {
    (void)other;
    throw std::runtime_error("hash equal not implemented for this Value type");
}

Value::Ptr Value::nextFromIter(std::shared_ptr<Value::IteratorState> state) const {
    (void)state;
    throw std::runtime_error("iterators not implemented for this Value type");
}

std::shared_ptr<Value::IteratorState> Value::iterInitialState() const {
    throw std::runtime_error("iterators not implemented for this Value type");
}


size_t Value::len() const {
    throw std::runtime_error("len not implemented for this Value type");
};

Value::Ptr Value::clone() const {
    throw std::runtime_error("clone not implemented for this Value type");
}

std::string Value::toString() const {
    throw std::runtime_error("toCode not implemented for this Value type");
}

const std::string& Value::typeString() const {
    return ValueMetadata::GetInstance().GetValueName(type);
}

Value::Ptr Value::copy(Value::Ptr value) {
    if (value->isPrimitive()) {
        return value->clone();
    }
    return value;
}

const ValueMetadata& ValueMetadata::GetInstance() {
  static ValueMetadata instance;
  return instance;
}

const std::string& ValueMetadata::GetValueName(Value::Type value) const {
  return value_names[static_cast<size_t>(value)];
}