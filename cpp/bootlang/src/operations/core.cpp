#include "operations/core.hpp"

std::string core_print(const std::vector<Value::Ptr>& args) {  
  std::ostringstream out;
  bool empty = true;

  for (Value::Ptr value : args) {
    if (!empty) {
      out << " ";
    }

    out << value->toString();

    empty = false;
  }

  out << std::endl;

  return out.str();
}

Value::Ptr core_range(Value::CallableInfo& info) {
  if (info.args.empty() || info.args.size() > 3) {
    throw std::runtime_error("range expects 1-3 arguments");
  }

  for (size_t i = 0; i < info.args.size(); i++) {
    if (info.args.at(i)->type != Value::Type::INT) {
      throw std::runtime_error("range only accept values of type int");
    }
  }

  return std::make_shared<BuiltinGeneratorValue>(
    core_range_init, core_range_next, std::move(info)
  );
}

std::shared_ptr<Value::IteratorState> core_range_init(const Value::CallableInfo& info) {
  auto state = std::make_shared<RangeIterState>();

  if (info.args.size() == 1) {
    state->end = Value::toDerived<IntValue>(info.args.at(0))->value;
    if (state->end <= 0) state->finished = true;

    return state;
  }

  state->start = Value::toDerived<IntValue>(info.args.at(0))->value;
  state->end = Value::toDerived<IntValue>(info.args.at(1))->value;

  if (info.args.size() == 3) {
    state->step = Value::toDerived<IntValue>(info.args.at(2))->value;
  }

  if (state->step == 0) state->finished = true;
  if (state->step > 0 && state->start >= state->end) state->finished = true;
  if (state->step < 0 && state->start <= state->end) state->finished = true;

  return state;
}

Value::Ptr core_range_next(std::shared_ptr<Value::IteratorState> base_state) {
  if (base_state->finished) {
      return NoneValue::NONE;
  }

  auto state = std::static_pointer_cast<RangeIterState>(base_state);

  auto next = std::make_shared<IntValue>(state->start);
  state->start += state->step;

  if (state->step > 0 && state->start >= state->end) state->finished = true;
  if (state->step < 0 && state->start <= state->end) state->finished = true;
  return next;
}

Value::Ptr core_len(Value::CallableInfo& info) {
  if (info.args.size() != 1) {
    throw std::runtime_error("Function expects one argument");
  }

  Value::Ptr collection = info.args.back();

  if (!collection->hasLength()) {
    throw std::runtime_error("Value isn't of type collection");
  }

  return std::make_shared<IntValue>(collection->len());
}

MapIterState::MapIterState(Value::Ptr function, std::vector<Value::Ptr> collections,
  std::vector<std::shared_ptr<Value::IteratorState>> iters, Value::VMCallback vm_call)
  : function(function), collections(collections), iters(iters), vm_call(vm_call) {}

Value::Ptr core_map(Value::CallableInfo& info) {
  if (info.args.size() < 2) {
    throw std::runtime_error("Function expects at least two arguments");
  }

  if (!info.args.at(0)->isCallable()) {
    throw std::runtime_error("Expected callable first argument");
  }

  for (size_t i = 1; i < info.args.size(); i++) {    
    if (!info.args.at(i)->isIterable()) {
      throw std::runtime_error("Expected iterable values");
    }
  }

  return std::make_shared<BuiltinGeneratorValue>(
    core_map_init, core_map_next, std::move(info)
  );
}

std::shared_ptr<Value::IteratorState> core_map_init(const Value::CallableInfo& info) {
  std::vector<Value::Ptr> collections;
  std::vector<std::shared_ptr<IterableValue::IteratorState>> iters;

  bool iter_has_finished = false;
  for (size_t i = 1; i < info.args.size(); i++) {
    auto iter = info.args.at(i)->iterInitialState();
    if (iter->finished) iter_has_finished = true;
    collections.push_back(info.args.at(i));
    iters.push_back(iter);
  }

  Value::Ptr func = info.args.at(0);

  auto it = std::make_shared<MapIterState>(
    func, collections, iters, info.vm_call
  );

  it->finished = iter_has_finished;
  return it;
}

Value::Ptr core_map_next(std::shared_ptr<Value::IteratorState> base_state) {
  if (base_state->finished) {
      return NoneValue::NONE;
  }

  auto state = std::static_pointer_cast<MapIterState>(base_state);
  auto none = NoneValue::NONE->clone();

  std::vector<Value::Ptr> func_args;
  for (size_t i = 0; i < state->collections.size(); i++) {
    auto iter = state->iters.at(i);
    Value::Ptr func_arg = state->collections.at(i)->nextFromIter(iter);
    if (iter->finished) state->finished = true;
    func_args.push_back(func_arg);
  }

  Value::Ptr result = state->vm_call(state->function, none, std::move(func_args));
  return result;
}


Value::Ptr core_sum(Value::CallableInfo& info) {
  if (info.args.empty() || info.args.size() > 2) {
    throw std::runtime_error("Function expects one or two arguments");
  }

  if (!info.args.at(0)->isIterable()) {
    throw std::runtime_error("Expected iterable values");
  }

  bool use_float = false;
  long isum = 0;
  double fsum = 0.0;

  if (info.args.size() == 2) {
    Value::Ptr start_value = info.args.at(1);
    if (start_value->type == Value::Type::INT) {
      isum += Value::toDerived<IntValue>(start_value)->value;
    } else if (start_value->type == Value::Type::FLOAT) {
      fsum += Value::toDerived<FloatValue>(start_value)->value;
      use_float = true;
    } else {
      throw std::runtime_error("start argument must be a number");
    }
  }
  
  Value::Ptr iterable = info.args.at(0);
  auto iter = iterable->iterInitialState();

  Value::Ptr toAdd = nullptr;

  while (!iter->finished && !use_float) {
    Value::Ptr toAdd = iterable->nextFromIter(iter);
    if (toAdd->type == Value::Type::INT) {
      isum += Value::toDerived<IntValue>(toAdd)->value;
    } else {
      break;
    }
  }

  if (iter->finished) {
    return std::make_shared<IntValue>(isum);
  }
  
  fsum += isum;

  if (toAdd != nullptr) {
    if (toAdd->type == Value::Type::FLOAT) {
      fsum += Value::toDerived<FloatValue>(toAdd)->value;
    } else {
      throw std::runtime_error("Attempted to sum a non-numerical value \"" + toAdd->toString() + "\"");
    }
  }

  while (!iter->finished) {
    Value::Ptr toAdd = iterable->nextFromIter(iter);
    if (toAdd->type == Value::Type::INT) {
      fsum += Value::toDerived<IntValue>(toAdd)->value;
    } else if (toAdd->type == Value::Type::FLOAT) {
      fsum += Value::toDerived<FloatValue>(toAdd)->value;
    } else {
      throw std::runtime_error("Attempted to sum a non-numerical value \"" + toAdd->toString() + "\"");
    }
  }
  
  return std::make_shared<FloatValue>(fsum);
}