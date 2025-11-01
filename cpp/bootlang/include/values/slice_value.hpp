#pragma once

#include <sstream>
#include "value.hpp"

class SliceValue : public Value {
public:
  long start, end, step;

  SliceValue(long start, long end, long step = 1);
  virtual bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toCode() const override;
};