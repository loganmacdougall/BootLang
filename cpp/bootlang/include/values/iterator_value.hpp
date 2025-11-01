#pragma once

#include <vector>
#include "value.hpp"

class IterableValue : public Value {
  public:
    Value::Ptr parent;
    std::shared_ptr<IteratorState> state;

    IterableValue(Value::Ptr parent, std::shared_ptr<IteratorState> state);
    bool isIterable() const override { return true; }
    virtual Value::Ptr next() const;
    virtual std::string toCode() const override;
};