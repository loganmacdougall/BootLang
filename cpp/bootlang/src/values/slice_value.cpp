#include "values/slice_value.hpp"

SliceValue::SliceValue(long start, long end, long step) 
: Value(Value::Type::SLICE), start(start), end(end), step(step) {}

bool SliceValue::equal(const Value& other) const {
  if (other.type != Value::Type::SLICE) return false;

  const SliceValue& other_slice = static_cast<const SliceValue&>(other);
  return start == other_slice.start &&
    end == other_slice.end &&
    step == other_slice.step;
}

Value::Ptr SliceValue::clone() const {
  return std::make_shared<SliceValue>(start, end, step);
}

std::string SliceValue::toCode() const {
  std::ostringstream out;

  out << "[" << start << ":" << end;

  if (step != 1) {
    out << ":" << step;
  }

  out << "]";

  return out.str();
}
