#pragma once

#include <stdexcept>
#include "values/all_values.hpp"
#include "token.hpp"

void list_append(std::shared_ptr<ListValue> a, Value::Ptr b);
void list_extend(std::shared_ptr<ListValue> a, Value::Ptr b);

Value::Ptr collection_index(Value::Ptr a_base, Value::Ptr b_base);
void collection_index_assign(Value::Ptr a_base, Value::Ptr b_base, Value::Ptr value);

