#include <vector>
#include "values/bool_value.hpp"

Value::Ptr bool_or_bool(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr bool_and_bool(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr not_bool(Value::Ptr a_base);
Value::Ptr to_bool(Value::CallableInfo& info);