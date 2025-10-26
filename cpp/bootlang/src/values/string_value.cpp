#include "values/string_value.hpp"

StringValue::StringValue(const std::string &value) : Value(Value::Type::STRING) ,value(value) {}

std::size_t StringValue::hash() const {
    return std::hash<std::string>()(value);
}

bool StringValue::equal(const Value& other) const {
    if (type != other.type) {
        return false;
    }

    const StringValue& other_string = static_cast<const StringValue&>(other);
    return value == other_string.value;
}