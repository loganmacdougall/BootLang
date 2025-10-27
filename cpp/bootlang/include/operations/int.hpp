#pragma once

#include <stdexcept>
#include "values/all_values.hpp"
#include "token.hpp"

Value::Ptr int_add_int(const Value& a_base, const Value& b_base);
Value::Ptr int_add_float(const Value& a_base, const Value& b_base);
Value::Ptr int_minus_int(const Value& a_base, const Value& b_base);
Value::Ptr int_minus_float(const Value& a_base, const Value& b_base);
Value::Ptr int_times_int(const Value& a_base, const Value& b_base);
Value::Ptr int_times_float(const Value& a_base, const Value& b_base);
Value::Ptr int_int_divide_int(const Value& a_base, const Value& b_base);
Value::Ptr int_int_divide_float(const Value& a_base, const Value& b_base);
Value::Ptr int_divide_int(const Value& a_base, const Value& b_base);
Value::Ptr int_divide_float(const Value& a_base, const Value& b_base);
Value::Ptr int_to_float(const Value& a_base);