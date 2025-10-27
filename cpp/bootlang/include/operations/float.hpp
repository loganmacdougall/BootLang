#pragma once

#include <stdexcept>
#include "values/all_values.hpp"
#include "token.hpp"

Value::Ptr float_add_float(const Value& a_base, const Value& b_base);
Value::Ptr float_add_int(const Value& a_base, const Value& b_base);
Value::Ptr float_minus_float(const Value& a_base, const Value& b_base);
Value::Ptr float_minus_int(const Value& a_base, const Value& b_base);
Value::Ptr float_times_float(const Value& a_base, const Value& b_base);
Value::Ptr float_times_int(const Value& a_base, const Value& b_base);
Value::Ptr float_divide_float(const Value& a_base, const Value& b_base);
Value::Ptr float_divide_int(const Value& a_base, const Value& b_base);
Value::Ptr float_int_divide_float(const Value& a_base, const Value& b_base);
Value::Ptr float_int_divide_int(const Value& a_base, const Value& b_base);
Value::Ptr float_to_int(const Value& a_base);