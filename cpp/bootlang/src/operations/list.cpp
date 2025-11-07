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

Value::Ptr collection_index(Value::Ptr a_base, Value::Ptr b_base) {
  switch (a_base->type) {
    case Value::Type::TUPLE:
    case Value::Type::LIST: {
      std::vector<Value::Ptr>* elems = nullptr;
      if (a_base->type == Value::Type::LIST) {
        std::shared_ptr<ListValue> a = Value::toDerived<ListValue>(a_base);
        elems = &a->elems;
      } else {
        std::shared_ptr<TupleValue> a = Value::toDerived<TupleValue>(a_base);
        elems = &a->elems;
      }
      
      if (b_base->type == Value::Type::INT) {
        std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
        return Value::copy((*elems)[b->value]);
      } else if (b_base->type == Value::Type::SLICE) {
        std::shared_ptr<SliceValue> b = Value::toDerived<SliceValue>(b_base);
        std::vector<Value::Ptr> slice_elems;
        auto sliceGen = b->sliceGenerator(elems->size());

        for (size_t i = sliceGen(); i != static_cast<size_t>(-1); i = sliceGen()) {
          slice_elems.push_back((*elems)[i]);
        }

        if (a_base->type == Value::Type::LIST) {
          return std::make_shared<ListValue>(std::move(slice_elems));
        } else {
          return std::make_shared<TupleValue>(std::move(slice_elems));
        }
      }

      throw std::runtime_error("Invalid subscript type for sequence");
    }

    case Value::Type::STRING: {
      std::shared_ptr<StringValue> a = Value::toDerived<StringValue>(a_base);
      const std::string &s = a->value;

      if (b_base->type == Value::Type::INT) {
        std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
        size_t idx = static_cast<size_t>(b->value < 0 ? s.size() - b->value : b->value);
        if (idx >= s.size()) {
          throw std::runtime_error("Index out of range");
        }
        std::string out(1, s[static_cast<size_t>(idx)]);
        return std::make_shared<StringValue>(std::move(out));
      }

      if (b_base->type == Value::Type::SLICE) {
        std::shared_ptr<SliceValue> b = Value::toDerived<SliceValue>(b_base);
        std::string out;
        auto sliceGen = b->sliceGenerator(s.size());
        for (size_t i = sliceGen(); i != static_cast<size_t>(-1); i = sliceGen()) {
          out.push_back(s[i]);
        }
        return std::make_shared<StringValue>(std::move(out));
      }

      throw std::runtime_error("Invalid subscript type for sequence");
    }

    case Value::Type::DICT:{
      std::shared_ptr<MapValue> a = Value::toDerived<MapValue>(a_base);
      return Value::copy(a->getValue(b_base));
    }

    default:
      throw std::runtime_error("Value type is not subscriptable");
  }
}

void collection_index_assign(Value::Ptr a_base, Value::Ptr b_base, Value::Ptr value) {
  if (a_base->type == Value::Type::DICT) {
    std::shared_ptr<MapValue> a = Value::toDerived<MapValue>(a_base);
    a->storeValue(b_base, value);
    return;
  }

  if (a_base->type != Value::Type::LIST) {
    throw std::runtime_error("Value type does not support item assignment");
  }

  std::shared_ptr<ListValue> a = Value::toDerived<ListValue>(a_base);

  if (b_base->type == Value::Type::INT) {
    std::shared_ptr<IntValue> b = Value::toDerived<IntValue>(b_base);
    size_t index = static_cast<size_t>(b->value ? a->elems.size() - b->value : b->value);
    if (index >= a->elems.size()) {
      throw std::runtime_error("Index out of bounds");
    }
    a->elems[b->value] = Value::copy(value);
    return;
  }

  if (b_base->type != Value::Type::SLICE) {
    throw std::runtime_error("Invalid subscript type for sequence");
  }

  std::shared_ptr<SliceValue> b = Value::toDerived<SliceValue>(b_base);

  size_t slice_length = b->sliceLength(a->elems.size());
  auto [start, end, step] = b->sliceValues(a->elems.size());

  if (step == 1) {
    if (slice_length == 0) return;

    auto iter_state = value->toIter();
  
    std::vector<Value::Ptr> elems;

    elems.insert(elems.end(), a->elems.begin(), a->elems.begin() + start);
    while (!iter_state->finished) {
      auto elem = value->next(iter_state);
      elems.push_back(Value::copy(elem)); 
    }
    elems.insert(elems.end(), a->elems.begin() + end, a->elems.end());

    std::swap(a->elems, elems);
    return;
  }

  auto [slice_start, slice_end] = b->sliceRange(a->elems.size());
  if (static_cast<size_t>(slice_end) >= a->elems.size() || slice_start < 0) {
    throw std::runtime_error("Slice subscripts outside the range of the list");
  }
  
  auto slice_gen = b->sliceGenerator(a->elems.size());
  auto iter_state = value->toIter();

  for (size_t i = slice_gen(); i != static_cast<size_t>(-1); i = slice_gen()) {
    a->elems[i] = value->next(iter_state);
  }
}

Value::Ptr to_list(Value::CallableInfo& info) {
  Value::Ptr iterable = info.args.at(0);

  if (iterable->type == Value::Type::LIST) {
    return iterable;
  }

  auto iter = iterable->toIter();
  auto list = std::make_shared<ListValue>();

  while (!iter->finished) {
    Value::Ptr value = iterable->next(iter);
    list->pushValue(value);
  }

  return list;
}