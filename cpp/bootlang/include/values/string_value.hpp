#pragma once

#include <string>
#include "value.hpp"
#include "values/none_value.hpp"

class StringValue : public Value {
  public:
    std::string value;

    struct IteratorState : public Value::IteratorState {
      std::string::const_iterator it;
    };

    StringValue(const std::string &value);
    virtual bool isHashable() const override { return true; }
    bool isIterable() const override { return true; }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
    virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> iterInitialState() const;
    virtual Value::Ptr clone() const override;
    virtual std::string toCode() const override;
};