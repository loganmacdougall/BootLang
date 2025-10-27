#pragma once

#include <stdexcept>
#include "values/all_values.hpp"
#include "token.hpp"

void list_append(std::shared_ptr<ListValue> a, Value::Ptr b);
void list_extend(std::shared_ptr<ListValue> a, Value::Ptr b);
