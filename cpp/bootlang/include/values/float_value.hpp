#pragma once

#include "value.hpp"

class FloatValue : public Value {
  public:
    double value;

    FloatValue(double value);
    virtual bool toBool() const override { return value != 0; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
    virtual Value::Ptr clone() const override;
    virtual std::string toString() const override;
};