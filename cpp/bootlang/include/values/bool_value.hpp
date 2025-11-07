#pragma once

#include "value.hpp"

class BoolValue : public Value {
  public:
    bool value;

    BoolValue(bool value);
    virtual bool toBool() const override { return value; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
    virtual Value::Ptr clone() const override;
    virtual std::string toString() const override;
};