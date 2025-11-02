#include "values/all_values.hpp"

Value::Ptr string_eq_string(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr string_add_string(Value::Ptr a_base, Value::Ptr b_base);

template<typename T>
Value::Ptr string_add_value(Value::Ptr a_base, Value::Ptr b_base) {
    std::shared_ptr<StringValue> a = Value::toDerived<StringValue>(a_base);
    std::shared_ptr<T> b = Value::toDerived<T>(b_base);
    return std::make_shared<StringValue>(a->value + b->toString());
}