#pragma once

#include <stdexcept>
#include "values/all_values.hpp"
#include "token.hpp"

Value::Ptr float_add_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_add_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_minus_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_minus_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_times_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_times_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_divide_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_divide_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_int_divide_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_int_divide_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_to_int(Value::Ptr a_base);