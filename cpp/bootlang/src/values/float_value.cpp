#include "values/float_value.hpp"

FloatValue::FloatValue(double value) : Value(Value::Type::FLOAT), value(value) {}

std::size_t FloatValue::hash() const {
    return std::hash<double>()(value);
}

bool FloatValue::equal(const Value& other) const {
    if (type != other.type) {
        return false;
    }

    const FloatValue& other_float = static_cast<const FloatValue&>(other);
    return value == other_float.value;
}