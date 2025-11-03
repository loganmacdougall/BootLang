#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include "values/all_values.hpp"

std::string core_print(const std::vector<Value::Ptr>& args);

Value::Ptr core_range(Value::Ptr& self, const std::vector<Value::Ptr>& args);
std::shared_ptr<Value::IteratorState> core_range_init(const std::vector<Value::Ptr>& args);
Value::Ptr core_range_next(std::shared_ptr<Value::IteratorState> base_state);

Value::Ptr core_len(Value::Ptr& self, const std::vector<Value::Ptr>& args);

class RangeIterState : public Value::IteratorState {
public:  
  long start = 0, end, step = 1;
};