#include "values/tuple_value.hpp"

TupleValue::TupleValue(std::vector<Value::Ptr>&& elems)
    : Value(Value::Type::TUPLE), elems(std::move(elems)) {}

std::size_t TupleValue::hash() const {
  size_t h = elems.size() ^ 0x5475706c65;
  for (const auto& elem : elems) {
    if (elem->isHashable()) {
      h ^= elem->hash() + 0x4d6f7265 + (h << 6) + (h >> 2);
    } else {
      throw std::runtime_error("Tuple contains non-hashable element");
    }
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