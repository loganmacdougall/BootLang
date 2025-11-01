#include "values/map_value.hpp"

MapValue::MapValue(std::map<Value::Ptr, Value::Ptr>&& map)
    : Value(Value::MAP), map(std::move(map)) {}

Value::Ptr MapValue::nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const {
    if (base_state->finished) {
        return NoneValue::NONE;
    }
    
    auto state = std::static_pointer_cast<MapValue::IteratorState>(base_state);

    Value::Ptr elem = (*state->it).first;
    state->it++;

    if (state->it == map.end()) {
        base_state->finished = true;
    }

    return elem;
}

std::shared_ptr<Value::IteratorState> MapValue::iterInitialState() const {
    auto iter_state = std::make_shared<MapValue::IteratorState>(
        MapValue::IteratorState()
    );

    iter_state->it = map.begin();

    if (iter_state->it == map.end()) {
        iter_state->finished = true;
    }

    return iter_state;
}


Value::Ptr MapValue::clone() const {
    std::map<Value::Ptr, Value::Ptr> other_map;

    for (auto &pair : map) {
        Value::Ptr key = pair.first->clone();
        Value::Ptr value = pair.second->clone();
        other_map[key] = value;
    }

    return std::make_shared<MapValue>(MapValue(std::move(other_map)));
}

std::string MapValue::toCode() const {
    std::string out = "[";

    for (auto it = map.begin(); it != map.end(); it++) {
        if (it != map.begin()) {
            out += ',';
        }

        if (out.size() > Value::LIST_DISPLAY_HALF_WIDTH) {
            out = out.substr(0, Value::LIST_DISPLAY_HALF_WIDTH);
            break;
        }
        
        out += it->first->toCode() + ":";

        if (out.size() > Value::LIST_DISPLAY_HALF_WIDTH) {
            out = out.substr(0, Value::LIST_DISPLAY_HALF_WIDTH);
            break;
        }

        out += it->second->toCode();
    }

    out += '}';
    return out;
}