#include "values/int_value.hpp"

IntValue::IntValue(long value) : Value(Value::Type::INT), value(value) {}

std::size_t IntValue::hash() const {
    return std::hash<long>()(value);
}

bool IntValue::equal(const Value& other) const {
    if (type != other.type) {
        return false;
    }

    const IntValue& other_int = static_cast<const IntValue&>(other);
    return value == other_int.value;
}

Value::Ptr IntValue::clone() const {
    return std::make_shared<IntValue>(IntValue(value));
}

std::string IntValue::toString() const {
    return std::to_string(value);
}