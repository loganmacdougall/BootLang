#include "values/list_value.hpp"

ListValue::ListValue(std::vector<Value::Ptr>&& elems)
    : Value(Value::LIST), elems(std::move(elems)) {}

Value::Ptr ListValue::nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const {
    if (base_state->finished) {
        return NoneValue::NONE;
    }
    
    auto state = std::static_pointer_cast<ListValue::IteratorState>(base_state);

    Value::Ptr elem = *state->it;
    state->it++;

    if (state->it == elems.end()) {
        base_state->finished = true;
    }

    return copy(elem);
}

std::shared_ptr<Value::IteratorState> ListValue::iterInitialState() const {
    auto iter_state = std::make_shared<ListValue::IteratorState>(
        ListValue::IteratorState()
    );

    iter_state->it = elems.begin();

    if (iter_state->it == elems.end()) {
        iter_state->finished = true;
    }

    return iter_state;
}

Value::Ptr ListValue::clone() const {
  std::vector<Value::Ptr> other_elems = elems;

  for (size_t i = 0; i < elems.size(); i++) {
    Value::Ptr copied = elems[i]->clone();
    other_elems.push_back(std::move(copied));
  }

  return std::make_shared<ListValue>(ListValue(std::move(other_elems)));
}

std::string ListValue::toString() const {
    std::string out = "[";

    for (auto it = elems.begin(); it != elems.end(); it++) {
        if (it != elems.begin()) {
            out += ',';
        }

        if (out.size() > Value::LIST_DISPLAY_HALF_WIDTH) {
            out = out.substr(0, Value::LIST_DISPLAY_HALF_WIDTH);
            break;
        }
        
        out += it->get()->toString();
    }

    out += ']';
    return out;
}