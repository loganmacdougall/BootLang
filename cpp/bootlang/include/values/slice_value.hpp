#pragma once

#include <functional>
#include <optional>
#include <sstream>
#include "value.hpp"

class SliceValue : public Value {
public:
  std::optional<long> start, end, step;

  SliceValue(std::optional<long> start, std::optional<long> end, std::optional<long> step = std::nullopt);
  std::tuple<long, long, long> sliceValues(size_t length) const;
  size_t sliceLength(size_t length) const;
  std::tuple<long, long> sliceRange(size_t length) const;
  std::function<size_t()> sliceGenerator(size_t length) const;
  
  virtual bool equal(const Value& other) const override;
  virtual Value::Ptr clone() const override;
  virtual std::string toString() const override;

};