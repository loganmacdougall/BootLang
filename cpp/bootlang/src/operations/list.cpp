#include "operations/list.hpp"

void list_append(std::shared_ptr<ListValue> a, Value::Ptr b) {
  a->elems.push_back(b);
}

void list_extend(std::shared_ptr<ListValue> a, Value::Ptr b) {
  switch (b->type) {
    case Value::Type::LIST: {
      const ListValue* list_b = Value::toDerived<ListValue>(b.get());

      for (size_t i = 0; i < list_b->elems.size(); i++) {
        a->elems.push_back(list_b->elems[i]);
      }

      break;
    }

    default:
      throw std::runtime_error("Can't extend non-iterable");
  }
}