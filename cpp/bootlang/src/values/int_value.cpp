#include "values/int_value.hpp"

IntValue::IntValue(int value) : Value(Value::Type::INT), value(value) {}

std::size_t IntValue::hash() const {
    return std::hash<int>()(value);
}

bool IntValue::equal(const Value& other) const {
    if (type != other.type) {
        return false;
    }

    const IntValue& other_int = static_cast<const IntValue&>(other);
    return value == other_int.value;
}