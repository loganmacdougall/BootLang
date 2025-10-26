#pragma once

#include <map>
#include "value.hpp"

class MapValue : public Value {
  public:
    std::map<Value::Ptr, Value::Ptr> map;

    MapValue(std::map<Value::Ptr, Value::Ptr>&& map);
};