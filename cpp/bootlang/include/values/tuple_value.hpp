#pragma once

#include <vector>
#include <stdexcept>
#include "value.hpp"
#include "values/none_value.hpp"

class TupleValue : public Value {
  public:
    std::vector<Value::Ptr> elems;

    struct IteratorState : public Value::IteratorState {
      std::vector<Value::Ptr>::const_iterator it;
    };

    TupleValue(std::vector<Value::Ptr>&& elems);
    virtual bool isHashable() const override { return true; }
    bool isIterable() const override { return true; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
    virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> iterInitialState() const;
    virtual Value clone() const override;
  
};