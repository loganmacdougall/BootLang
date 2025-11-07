#pragma once

#include <vector>
#include "value.hpp"

class IterableValue : public Value {
  public:
    Value::Ptr parent;
    std::shared_ptr<IteratorState> state;

    IterableValue(Value::Ptr parent, std::shared_ptr<IteratorState> state);
    virtual Value::Ptr next(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> toIter() const;
    virtual Value::Ptr next() const;
    virtual std::string toString() const override;
};