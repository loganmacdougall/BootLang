#include "values/list_value.hpp"

ListValue::ListValue(std::vector<Value::Ptr>&& elems)
    : Value(Value::LIST), elems(std::move(elems)) {}