#pragma once

#include "value.hpp"

class BoolValue : public Value {
  public:
    bool value;

    BoolValue(bool value);
    virtual bool isHashable() const override { return true; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
    virtual Value::Ptr clone() const override;
    virtual std::string toCode() const override;
};