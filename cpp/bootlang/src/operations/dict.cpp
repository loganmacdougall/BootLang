#include "operations/dict.hpp"

Value::Ptr dict_items(Value::CallableInfo& info) {
  return std::make_shared<BuiltinGeneratorValue>(
    dict_items_init, dict_items_next, std::move(info)
  );
}

std::shared_ptr<Value::IteratorState> dict_items_init(const Value::CallableInfo& info) {
  std::shared_ptr<MapValue> dict = Value::toDerived<MapValue>(info.self);
  
  auto iter_state = std::make_shared<DictItemsIterState>();
  iter_state->dict = dict;
  iter_state->it = dict->keys.begin();

  if (iter_state->it == dict->keys.end()) {
      iter_state->finished = true;
  }

  return iter_state;
}

Value::Ptr dict_items_next(std::shared_ptr<Value::IteratorState> base_state) {
    if (base_state->finished) {
        return NoneValue::NONE;
    }
    
    auto state = std::static_pointer_cast<DictItemsIterState>(base_state);

    Value::Ptr key = state->it->second;
    Value::Ptr value = state->dict->values.at(state->it->first);
    state->it++;

    if (state->it == state->dict->keys.end()) {
        base_state->finished = true;
    }

    std::vector<Value::Ptr> tuple_elems;
    tuple_elems.push_back(key);
    tuple_elems.push_back(value);

    return std::make_shared<TupleValue>(std::move(tuple_elems));
}
