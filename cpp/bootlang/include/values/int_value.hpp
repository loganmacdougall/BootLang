#pragma once

#include "value.hpp"

class IntValue : public Value {
  public:
    int value;

    IntValue(int value);
    virtual bool isHashable() const override { return true; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
};