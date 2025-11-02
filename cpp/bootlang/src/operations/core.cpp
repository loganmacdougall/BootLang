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

Value::Ptr core_range(const std::vector<Value::Ptr>& args) {
  std::vector<Value::Ptr> args_copy;

  for (auto arg : args) {
    args_copy.push_back(Value::copy(arg));
  }

  return std::make_shared<BuiltinGeneratorValue>(
    core_range_init, core_range_next, args_copy
  );
}

std::shared_ptr<Value::IteratorState> core_range_init(const std::vector<Value::Ptr>& args) {
  auto state = std::make_shared<RangeIterState>();

  if (args.empty() || args.size() > 3) {
    throw std::runtime_error("range expects 1-3 arguments");
  }

  for (size_t i = 0; i < args.size(); i++) {
    if (args.at(i)->type != Value::Type::INT) {
      throw std::runtime_error("range only accept values of type int");
    }
  }

  if (args.size() == 1) {
    state->end = Value::toDerived<IntValue>(args.at(0))->value;
    if (state->end <= 0) state->finished = true;

    return state;
  }

  state->start = Value::toDerived<IntValue>(args.at(0))->value;
  state->end = Value::toDerived<IntValue>(args.at(1))->value;

  if (args.size() == 3) {
    state->step = Value::toDerived<IntValue>(args.at(2))->value;
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