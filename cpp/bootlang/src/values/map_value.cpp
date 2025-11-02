#include "values/map_value.hpp"

MapValue::MapValue(std::vector<std::pair<Value::Ptr, Value::Ptr>>&& map_pairs)
    : Value(Value::DICT) {
    for (auto [key, value] : map_pairs) {
        storeValue(key, value);
    }
}

std::pair<size_t, size_t> MapValue::getKeyId(Value::Ptr key) {
    if (!key->isHashable()) {
        throw std::runtime_error("Attempted to use an unhashable value as dict key");
    }

    size_t key_h = key->hash();

    auto it = key_hashmap.find(key_h);
    if (it != key_hashmap.end()) {
        for (size_t ki : it->second) {
            Value::Ptr other_key = keys[ki];
            if (other_key->equal(*key.get())) {
                return std::pair(key_h, ki);
            }
        }
    }

    return std::pair(key_h, -1);
}
    

void MapValue::storeValue(Value::Ptr key, Value::Ptr value) {
    auto [key_h, id] = getKeyId(key);

    if (id != static_cast<size_t>(-1)) {
        values[id] = value;
        return;
    }

    key_hashmap.insert({{key_h, std::vector<size_t>()}});
    id = next_id++;

    key_hashmap[key_h].push_back(id);
    keys.insert({{id, copy(key)}});
    values.insert({{id, copy(value)}});
}

bool MapValue::hasValue(Value::Ptr key) {
    return getKeyId(key).second != static_cast<size_t>(-1);
}

Value::Ptr MapValue::getValue(Value::Ptr key) {
    auto [key_h, id] = getKeyId(key);

    if (id == static_cast<size_t>(-1)) {
        throw std::runtime_error("Key not in dictionary");
    }

    return values[id];

}

Value::Ptr MapValue::nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const {
    if (base_state->finished) {
        return NoneValue::NONE;
    }
    
    auto state = std::static_pointer_cast<MapValue::IteratorState>(base_state);

    Value::Ptr elem = state->it->second;
    state->it++;

    if (state->it == keys.end()) {
        base_state->finished = true;
    }

    return copy(elem);
}

std::shared_ptr<Value::IteratorState> MapValue::iterInitialState() const {
    auto iter_state = std::make_shared<MapValue::IteratorState>(
        MapValue::IteratorState()
    );

    iter_state->it = keys.begin();

    if (iter_state->it == keys.end()) {
        iter_state->finished = true;
    }

    return iter_state;
}


Value::Ptr MapValue::clone() const {
    std::vector<std::pair<Value::Ptr, Value::Ptr>> cloned_pairs;
    cloned_pairs.reserve(values.size());

    for (const auto& [id, value] : values) {
        Value::Ptr key_clone = copy(keys.at(id));
        Value::Ptr value_clone = copy(value);
        cloned_pairs.emplace_back(std::move(key_clone), std::move(value_clone));
    }

    return std::make_shared<MapValue>(std::move(cloned_pairs));
}

std::string MapValue::toString() const {
    std::string out = "[";

    for (auto it = values.begin(); it != values.end(); it++) {
        if (it != values.begin()) {
            out += ',';
        }

        Value::Ptr key = keys.at(it->first);
        Value::Ptr value = it->second;

        if (out.size() > Value::LIST_DISPLAY_HALF_WIDTH) {
            out = out.substr(0, Value::LIST_DISPLAY_HALF_WIDTH);
            break;
        }
        
        out += key->toString() + ":";

        if (out.size() > Value::LIST_DISPLAY_HALF_WIDTH) {
            out = out.substr(0, Value::LIST_DISPLAY_HALF_WIDTH);
            break;
        }

        out += value->toString();
    }

    out += '}';
    return out;
}