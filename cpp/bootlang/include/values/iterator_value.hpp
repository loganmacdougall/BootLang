#pragma once

#include <vector>
#include "value.hpp"

class IterableValue : public Value {
  public:
    Value::Ptr parent;
    IteratorState state;

    IterableValue(Value::Ptr parent, IteratorState&& state);
    bool isIterable() const override { return true; }
    virtual Value::Ptr next(IteratorState) const;
    virtual std::string toCode() const override;
};