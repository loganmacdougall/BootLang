#include "values/set_value.hpp"

SetValue::SetValue(std::vector<Value::Ptr>&& elems)
    : Value(Value::SET) {
    for (auto& key : elems) {
        storeKey(key);
    }
}

SetValue::SetValue() : Value(Value::SET) {}

std::pair<size_t, size_t> SetValue::getKeyId(Value::Ptr key) {
    if (!key->isHashable()) {
        throw std::runtime_error("Attempted to use an unhashable value as set element");
    }

    size_t key_h = key->hash();

    auto it = key_hashmap.find(key_h);
    if (it != key_hashmap.end()) {
        for (size_t ki : it->second) {
            Value::Ptr other_key = keys.at(ki);
            if (other_key->equal(*key)) {
                return std::pair(key_h, ki);
            }
        }
    }

    return std::pair(key_h, static_cast<size_t>(-1));
}

void SetValue::storeKey(Value::Ptr key) {
    auto [key_h, id] = getKeyId(key);

    if (id != static_cast<size_t>(-1)) {
        return;
    }

    size_t new_id = keys.size();

    key_hashmap[key_h].push_back(new_id);
    keys[new_id] = copy(key);
}

bool SetValue::hasKey(Value::Ptr key) {
    return getKeyId(key).second != static_cast<size_t>(-1);
}

Value::Ptr SetValue::nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const {
    if (base_state->finished) {
        return NoneValue::NONE;
    }
    
    auto state = std::static_pointer_cast<SetValue::IteratorState>(base_state);

    Value::Ptr elem = state->it->second;
    state->it++;
    
    if (state->it == keys.end()) {
        base_state->finished = true;
    }

    return copy(elem);
}

std::shared_ptr<Value::IteratorState> SetValue::iterInitialState() const {
    auto iter_state = std::make_shared<SetValue::IteratorState>(
        SetValue::IteratorState()
    );

    iter_state->it = keys.begin();

    if (iter_state->it == keys.end()) {
        iter_state->finished = true;
    }


    return iter_state;
}


Value::Ptr SetValue::clone() const {
    std::vector<Value::Ptr> cloned_elems;
    cloned_elems.reserve(keys.size());

    for (const auto& [id, key] : keys) {
        cloned_elems.push_back(copy(key));
    }

    return std::make_shared<SetValue>(std::move(cloned_elems));
}


std::string SetValue::toString() const {
    std::string out = "{";

    for (auto it = keys.begin(); it != keys.end(); it++) {
        if (it != keys.begin()) {
            out += ", ";
        }

        if (out.size() > Value::LIST_DISPLAY_HALF_WIDTH) {
            out = out.substr(0, Value::LIST_DISPLAY_HALF_WIDTH);
            break;
        }
        
        out += it->second.get()->toString();
    }

    out += '}';
    return out;
}