#pragma once

#include <vector>
#include <sstream>
#include "value.hpp"
#include "values/none_value.hpp"

class ListValue : public Value {
public:
  std::vector<Value::Ptr> elems;

  struct IteratorState : public Value::IteratorState {
    std::vector<Value::Ptr>::const_iterator it;
  };

  ListValue(std::vector<Value::Ptr>&& elems);
  bool isIterable() const override { return true; }
  virtual bool toBool() const { return !elems.empty(); }
  virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> state) const override;
  virtual std::shared_ptr<Value::IteratorState> iterInitialState() const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toCode() const override;
};