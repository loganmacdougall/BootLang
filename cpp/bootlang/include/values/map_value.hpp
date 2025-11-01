#pragma once

#include <map>
#include <sstream>
#include "value.hpp"
#include "values/none_value.hpp"

class MapValue : public Value {
  public:
    std::map<Value::Ptr, Value::Ptr> map;

    struct IteratorState : public Value::IteratorState {
      std::map<Value::Ptr, Value::Ptr>::const_iterator it;
    };

    MapValue(std::map<Value::Ptr, Value::Ptr>&& map);
    virtual bool toBool() const { return !map.empty(); }
    bool isIterable() const override { return true; }
    virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const override;
    virtual std::shared_ptr<Value::IteratorState> iterInitialState() const override;
    virtual Value::Ptr clone() const override;
    virtual std::string toCode() const override;
};