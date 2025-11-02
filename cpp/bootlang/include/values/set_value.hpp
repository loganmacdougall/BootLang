#pragma once

#include <map>
#include <vector>
#include <sstream>
#include "value.hpp"
#include "values/none_value.hpp"

class SetValue : public Value {
  public:
    std::map<size_t, std::vector<size_t>> key_hashmap;
    std::map<size_t, Value::Ptr> keys;

    struct IteratorState : public Value::IteratorState {
      std::map<size_t, Value::Ptr>::const_iterator it;
    };

    SetValue(std::vector<Value::Ptr>&& elems);
    std::pair<size_t, size_t> getKeyId(Value::Ptr key);
    void storeKey(Value::Ptr key);
    bool hasKey(Value::Ptr key);

    virtual bool toBool() const { return !keys.empty(); }
    bool isIterable() const override { return true; }
    virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> iterInitialState() const;
    virtual Value::Ptr clone() const override;
    virtual std::string toString() const override;
};