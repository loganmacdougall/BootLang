#include "values/set_value.hpp"

SetValue::SetValue(std::set<Value::Ptr>&& elems)
    : Value(Value::SET), elems(std::move(elems)) {}