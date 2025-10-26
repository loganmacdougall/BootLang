#pragma once

#include "value.hpp"

class FloatValue : public Value {
  public:
    double value;

    FloatValue(double value);
    virtual bool isHashable() const override { return true; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
};