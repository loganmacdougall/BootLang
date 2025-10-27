#include "values/map_value.hpp"

MapValue::MapValue(std::map<Value::Ptr, Value::Ptr>&& map)
    : Value(Value::MAP), map(std::move(map)) {}

Value::Ptr MapValue::nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const {
    auto state = std::static_pointer_cast<MapValue::IteratorState>(base_state);
    
    if (state->it == map.end()) {
        return NoneValue::NONE;
    }

    Value::Ptr elem = (*state->it).first;
    state->it++;

    return elem;
}

std::shared_ptr<Value::IteratorState> MapValue::iterInitialState() const {
    auto iter_state = std::make_shared<MapValue::IteratorState>(
        MapValue::IteratorState()
    );

    iter_state->it = map.begin();

    return iter_state;
}


Value MapValue::clone() const {
    std::map<Value::Ptr, Value::Ptr> other_map;

    for (auto &pair : map) {
        Value::Ptr key = std::make_shared<Value>(pair.first->clone());
        Value::Ptr value = std::make_shared<Value>(pair.second->clone());
        other_map[key] = value;
    }

    return MapValue(std::move(other_map));
}