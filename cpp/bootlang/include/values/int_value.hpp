#pragma once

#include "value.hpp"

class IntValue : public Value {
  public:
    long value;

    IntValue(long value);
    virtual bool isHashable() const override { return true; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
    virtual Value clone() const override;
};