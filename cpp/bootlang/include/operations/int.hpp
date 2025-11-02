#pragma once

#include <stdexcept>
#include "values/all_values.hpp"
#include "token.hpp"

Value::Ptr int_negitive(Value::Ptr a_base);

Value::Ptr int_add_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_add_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_minus_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_minus_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_times_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_times_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_int_divide_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_int_divide_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_divide_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_divide_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_to_float(Value::Ptr a_base);

Value::Ptr int_ge_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_g_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_le_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_l_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_eq_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_neq_int(Value::Ptr a_base, Value::Ptr b_base);

Value::Ptr int_ge_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_g_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_le_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_l_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_eq_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr int_neq_float(Value::Ptr a_base, Value::Ptr b_base);