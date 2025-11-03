#pragma once

#include <stdexcept>
#include <cmath>
#include "values/all_values.hpp"
#include "token.hpp"

Value::Ptr float_negitive(Value::Ptr a_base);

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
Value::Ptr float_mod_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_mod_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_to_int(Value::Ptr a_base);

Value::Ptr float_ge_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_g_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_le_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_l_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_eq_float(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_neq_float(Value::Ptr a_base, Value::Ptr b_base);

Value::Ptr float_ge_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_g_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_le_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_l_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_eq_int(Value::Ptr a_base, Value::Ptr b_base);
Value::Ptr float_neq_int(Value::Ptr a_base, Value::Ptr b_base);