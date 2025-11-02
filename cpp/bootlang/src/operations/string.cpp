#include "operations/string.hpp"

Value::Ptr string_eq_string(Value::Ptr a_base, Value::Ptr b_base) {
    std::shared_ptr<StringValue> a = Value::toDerived<StringValue>(a_base);
    std::shared_ptr<StringValue> b = Value::toDerived<StringValue>(b_base);
    return std::make_shared<BoolValue>(a->value.compare(b->value) == 0);
}

Value::Ptr string_add_string(Value::Ptr a_base, Value::Ptr b_base) {
    std::shared_ptr<StringValue> a = Value::toDerived<StringValue>(a_base);
    std::shared_ptr<StringValue> b = Value::toDerived<StringValue>(b_base);
    return std::make_shared<StringValue>(a->value + b->value);
}