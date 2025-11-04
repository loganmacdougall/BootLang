#pragma once

#include <vector>
#include "value.hpp"

class IterableValue : public Value {
  public:
    Value::Ptr parent;
    std::shared_ptr<IteratorState> state;

    IterableValue(Value::Ptr parent, std::shared_ptr<IteratorState> state);
    bool isIterable() const override { return true; }
    virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> iterInitialState() const;
    virtual Value::Ptr next() const;
    virtual std::string toString() const override;
};