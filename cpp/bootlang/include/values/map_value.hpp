#pragma once

#include <map>
#include <vector>
#include <sstream>
#include "value.hpp"
#include "values/none_value.hpp"

class MapValue : public Value {
  public:
    std::map<size_t, std::vector<size_t>> key_hashmap;
    std::map<size_t, Value::Ptr> keys;
    std::map<size_t, Value::Ptr> values;
    size_t next_id = 0;

    struct IteratorState : public Value::IteratorState {
      std::map<size_t, Value::Ptr>::const_iterator it;
    };

    MapValue(std::vector<std::pair<Value::Ptr, Value::Ptr>>&& map_pairs);
    MapValue();
    std::pair<size_t, size_t> getKeyId(Value::Ptr key);
    void storeValue(Value::Ptr key, Value::Ptr value);
    bool hasValue(Value::Ptr key);
    Value::Ptr getValue(Value::Ptr key);

    bool isIterable() const override { return true; }
    virtual bool hasLength() const override { return true; }
    virtual bool toBool() const { return !keys.empty(); }
    virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const override;
    virtual std::shared_ptr<Value::IteratorState> iterInitialState() const override;
    virtual size_t len() const override { return keys.size(); };
    virtual Value::Ptr clone() const override;
    virtual std::string toString() const override;
};