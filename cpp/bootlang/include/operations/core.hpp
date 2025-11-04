#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include "values/all_values.hpp"

std::string core_print(const std::vector<Value::Ptr>& args);

Value::Ptr core_range(Value::CallableInfo& info);
std::shared_ptr<Value::IteratorState> core_range_init(const Value::CallableInfo& info);
Value::Ptr core_range_next(std::shared_ptr<Value::IteratorState> base_state);

class RangeIterState : public Value::IteratorState {
public:  
  long start = 0, end, step = 1;
};

Value::Ptr core_len(Value::CallableInfo& info);

Value::Ptr core_map(Value::CallableInfo& info);
std::shared_ptr<Value::IteratorState> core_map_init(const Value::CallableInfo& info);
Value::Ptr core_map_next(std::shared_ptr<Value::IteratorState> base_state);

class MapIterState : public Value::IteratorState {
public:  
  Value::Ptr function;
  std::vector<Value::Ptr> collections;
  std::vector<std::shared_ptr<Value::IteratorState>> iters;
  Value::VMCallback vm_call;

  MapIterState(Value::Ptr function, std::vector<Value::Ptr> collections,
  std::vector<std::shared_ptr<Value::IteratorState>> iters, Value::VMCallback vm_call);
};

Value::Ptr core_sum(Value::CallableInfo& info);