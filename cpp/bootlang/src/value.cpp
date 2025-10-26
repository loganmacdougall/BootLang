#include "value.hpp"

size_t Value::hash() const {
    throw std::runtime_error("Hash function not implemented for this Value type");
}

bool Value::equal(const Value& other) const {
    (void)other;
    throw std::runtime_error("Equal hash function not implemented for this Value type");
}