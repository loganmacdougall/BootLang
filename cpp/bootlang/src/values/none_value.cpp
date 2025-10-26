#include "values/none_value.hpp"

NoneValue::NoneValue() : Value(Value::Type::NONE) {}

std::size_t NoneValue::hash() const {
    return 0x4e6f6e56616c7565;
}

bool NoneValue::equal(const Value& other) const {
    return type == other.type;
}