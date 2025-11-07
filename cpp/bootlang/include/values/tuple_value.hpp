#pragma once

#include <vector>
#include <sstream>
#include <stdexcept>
#include "value.hpp"
#include "values/none_value.hpp"

class TupleValue : public Value {
  public:
    std::vector<Value::Ptr> elems;

    struct IteratorState : public Value::IteratorState {
      std::vector<Value::Ptr>::const_iterator it;
    };

    TupleValue();
    TupleValue(std::vector<Value::Ptr>&& elems);
    virtual bool toBool() const { return !elems.empty(); }
    std::size_t hash() const override;
    bool equal(const Value& other) const override;
    virtual Value::Ptr next(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> toIter() const;
    virtual size_t len() const override { return elems.size(); };
    virtual Value::Ptr clone() const override;
    virtual std::string toString() const override;
  
};