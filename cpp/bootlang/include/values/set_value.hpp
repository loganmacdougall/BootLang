#pragma once

#include <set>
#include <sstream>
#include "value.hpp"
#include "values/none_value.hpp"

class SetValue : public Value {
  public:
    std::set<Value::Ptr> elems;

    struct IteratorState : public Value::IteratorState {
      std::set<Value::Ptr>::const_iterator it;
    };

    SetValue(std::set<Value::Ptr>&& elems);
    bool isIterable() const override { return true; }
    virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> iterInitialState() const;
    virtual Value::Ptr clone() const override;
    virtual std::string toCode() const override;
};