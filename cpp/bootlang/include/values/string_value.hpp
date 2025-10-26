#pragma once

#include <string>
#include "value.hpp"

class StringValue : public Value {
  public:
    std::string value;

    StringValue(const std::string &value);
    virtual bool isHashable() const override { return true; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
};