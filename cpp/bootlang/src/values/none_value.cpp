#include "values/none_value.hpp"

std::shared_ptr<NoneValue> NoneValue::NONE = std::make_shared<NoneValue>(NoneValue());

NoneValue::NoneValue() : Value(Value::Type::NONE) {}

std::size_t NoneValue::hash() const {
    return 0x4e6f6e56616c7565;
}

bool NoneValue::equal(const Value& other) const {
    return type == other.type;
}

Value NoneValue::clone() const {
    return NoneValue();
}