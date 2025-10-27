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

Value::Ptr StringValue::nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const {
    auto state = std::static_pointer_cast<StringValue::IteratorState>(base_state);
    
    if (state->it == value.end()) {
        return NoneValue::NONE;
    }

    char charater = *state->it;
    Value::Ptr elem = std::make_shared<StringValue>(StringValue(std::string(1, charater)));
    state->it++;

    return elem;
}

std::shared_ptr<Value::IteratorState> StringValue::iterInitialState() const {
    auto iter_state = std::make_shared<StringValue::IteratorState>(
        StringValue::IteratorState()
    );

    iter_state->it = value.begin();

    return iter_state;
}


Value StringValue::clone() const {
    return StringValue(value);
}