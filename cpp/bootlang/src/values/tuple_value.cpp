#include "values/tuple_value.hpp"

TupleValue::TupleValue(std::vector<Value::Ptr>&& elems)
    : Value(Value::Type::TUPLE), elems(std::move(elems)) {}

TupleValue::TupleValue() : Value(Value::Type::TUPLE) {}

std::size_t TupleValue::hash() const {
  size_t h = elems.size() ^ 0x5475706c65;
  for (const auto& elem : elems) {
    h ^= elem->hash() + 0x4d6f7265 + (h << 6) + (h >> 2);
  }
  return h;
}

bool TupleValue::equal(const Value& other) const {
  if (type != other.type) {
    return false;
  }

  const TupleValue& otherTuple = static_cast<const TupleValue&>(other);

  if (this->elems.size() != otherTuple.elems.size()) {
    return false;
  }
  
  for (size_t i = 0; i < this->elems.size(); ++i) {
    if (this->elems[i]->type != otherTuple.elems[i]->type) {
      return false;
    }
    
    auto thisElemHashable = std::static_pointer_cast<Value>(this->elems[i]);
    auto otherElemHashable = std::static_pointer_cast<Value>(otherTuple.elems[i]);
    if (!thisElemHashable || !otherElemHashable || !thisElemHashable->equal(*otherElemHashable)) {
      return false;
    }
  }
  return true;
}

Value::Ptr TupleValue::next(std::shared_ptr<Value::IteratorState> base_state) const {
    if (base_state->finished) {
        return NoneValue::NONE;
    }
    
    auto state = std::static_pointer_cast<TupleValue::IteratorState>(base_state);

    Value::Ptr elem = *state->it;
    state->it++;

    if (state->it == elems.end()) {
        base_state->finished = true;
    }

    return copy(elem);
}

std::shared_ptr<Value::IteratorState> TupleValue::toIter() const {
  auto iter_state = std::make_shared<TupleValue::IteratorState>(
      TupleValue::IteratorState()
  );

  iter_state->it = elems.begin();

  if (iter_state->it == elems.end()) {
      iter_state->finished = true;
  }

  return iter_state;
}


Value::Ptr TupleValue::clone() const {
  std::vector<Value::Ptr> other_elems;

  for (size_t i = 0; i < elems.size(); i++) {
    Value::Ptr copied = copy(elems[i]);
    other_elems.push_back(std::move(copied));
  }

  return std::make_shared<TupleValue>(TupleValue(std::move(other_elems)));
}

std::string TupleValue::toString() const {
    std::string out = "(";

    for (auto it = elems.begin(); it != elems.end(); it++) {
        if (it != elems.begin()) {
            out += ", ";
        }

        if (out.size() > Value::LIST_DISPLAY_HALF_WIDTH) {
            out = out.substr(0, Value::LIST_DISPLAY_HALF_WIDTH);
            break;
        }
        
        out += it->get()->toString();
    }

    out += ')';
    return out;
}