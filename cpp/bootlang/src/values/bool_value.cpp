#include "values/bool_value.hpp"

BoolValue::BoolValue(bool value) : Value(Value::Type::BOOL), value(value) {}

std::size_t BoolValue::hash() const {
    return std::hash<bool>{}(value);
}

bool BoolValue::equal(const Value& other) const {
    if (type != other.type) {
        return false;
    }

    const BoolValue& other_bool = static_cast<const BoolValue&>(other);
    return value == other_bool.value;
}

Value BoolValue::clone() const {
    return BoolValue(value);
}