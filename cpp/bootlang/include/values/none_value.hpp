#pragma once

#include "value.hpp"

class NoneValue : public Value {
public:
  
  NoneValue();
  std::size_t hash() const override;
  bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toString() const override;

  static std::shared_ptr<NoneValue> NONE;
};