#pragma once

#include <set>
#include "value.hpp"

class SetValue : public Value {
  public:
    std::set<Value::Ptr> elems;

    SetValue(std::set<Value::Ptr>&& elems);
};