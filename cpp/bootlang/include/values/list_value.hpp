#pragma once

#include <vector>
#include "value.hpp"

class ListValue : public Value {
  public:
    std::vector<Value::Ptr> elems;

    ListValue(std::vector<Value::Ptr>&& elems);
};