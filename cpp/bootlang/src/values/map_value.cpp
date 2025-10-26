#include "values/map_value.hpp"

MapValue::MapValue(std::map<Value::Ptr, Value::Ptr>&& map)
    : Value(Value::MAP), map(std::move(map)) {}