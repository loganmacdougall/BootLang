#pragma once

#include "value.hpp"

class NoneValue : public Value {
public:
  
  NoneValue();
  virtual bool isHashable() const override { return true; }
  std::size_t hash() const override;
  bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toCode() const override;

  static std::shared_ptr<NoneValue> NONE;
};