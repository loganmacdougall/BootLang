#pragma once

#include <vector>
#include <stdexcept>
#include "value.hpp"

class TupleValue : public Value {
  public:
    std::vector<Value::Ptr> elems;

    TupleValue(std::vector<Value::Ptr>&& elems);
    virtual bool isHashable() const override { return true; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
};