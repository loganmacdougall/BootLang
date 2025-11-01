#include "operations/list.hpp"

void list_append(Value::Ptr a_base, Value::Ptr b) {
  std::shared_ptr<ListValue> a = Value::toDerived<ListValue>(a_base);
  a->elems.push_back(b);
}

void list_extend(Value::Ptr a_base, Value::Ptr b) {
  std::shared_ptr<ListValue> a = Value::toDerived<ListValue>(a_base);
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