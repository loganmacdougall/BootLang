#include "values/set_value.hpp"

SetValue::SetValue(std::set<Value::Ptr>&& elems)
    : Value(Value::SET), elems(std::move(elems)) {}

Value::Ptr SetValue::nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const {
    auto state = std::static_pointer_cast<SetValue::IteratorState>(base_state);
    
    if (state->it == elems.end()) {
        return NoneValue::NONE;
    }

    Value::Ptr elem = *state->it;
    state->it++;

    return elem;
}

std::shared_ptr<Value::IteratorState> SetValue::iterInitialState() const {
    auto iter_state = std::make_shared<SetValue::IteratorState>(
        SetValue::IteratorState()
    );

    iter_state->it = elems.begin();

    return iter_state;
}


Value::Ptr SetValue::clone() const {
    std::set<Value::Ptr> other_elems;

    for (auto &elem : elems) {
        Value::Ptr copy = elem->clone();
        other_elems.insert(copy);
    }

    return std::make_shared<SetValue>(SetValue(std::move(other_elems)));
}

std::string SetValue::toCode() const {
    std::string out = "{";

    for (auto it = elems.begin(); it != elems.end(); it++) {
        if (it != elems.begin()) {
            out += ',';
        }

        if (out.size() > Value::LIST_DISPLAY_HALF_WIDTH) {
            out = out.substr(0, Value::LIST_DISPLAY_HALF_WIDTH);
            break;
        }
        
        out += it->get()->toCode();
    }

    out += '}';
    return out;
}