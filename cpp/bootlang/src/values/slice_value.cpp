#include "values/slice_value.hpp"

SliceValue::SliceValue(std::optional<long> start, std::optional<long> end, std::optional<long> step)
: Value(Value::Type::SLICE), start(start), end(end), step(step) {}

std::tuple<long, long, long> SliceValue::sliceValues(size_t length) const {
  long _start = start.value_or(0);
  long _end = end.value_or(length);
  long _step = step.value_or(1);
  
  _start = _start < 0 ? length - _start : _start;
  _end = _end < 0 ? length + _end : _end;

  return std::tuple(_start, _end, _step);
}

size_t SliceValue::sliceLength(size_t length) const {
  auto [_start, _end, _step] = sliceValues(length);

  if (_step == 0) return 0;
  if (_step > 0 && _start >= _end) return 0;
  if (_step < 0 && _start <= _end ) return 0;
  
  return (_end - _start + 1) / _step;
}

std::tuple<long, long> SliceValue::sliceRange(size_t length) const {
  auto [_start, _end, _step] = sliceValues(length);

  if (_step == 0) return std::tuple(0,0);
  if (_step > 0 && _start >= _end) return std::tuple(0,0);
  if (_step < 0 && _start <= _end ) return std::tuple(0,0);

  long count = (_end - _start - 1) / _step;
  return std::tuple(_start, _start + (_step * count));
}

std::function<size_t()> SliceValue::sliceGenerator(size_t length) const {
  auto slice_values = sliceValues(length);
  long _start = std::get<0>(slice_values);
  long _end = std::get<1>(slice_values);
  long _step = std::get<2>(slice_values);

  return [_start, _end, _step]() mutable {
    if (_step == 0) return static_cast<size_t>(-1);
    if (_step > 0 && _start >= _end) return static_cast<size_t>(-1);
    if (_step < 0 && _start <= _end) return static_cast<size_t>(-1);

    const size_t next = static_cast<size_t>(_start);
    _start += _step;
    return next;
  };
}

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

std::string SliceValue::toString() const {
  std::ostringstream out;

  out << "[" << (start.has_value() ? std::to_string(start.value()) : "");
  out << ":" << (end.has_value() ? std::to_string(end.value()) : "");

  if (step.has_value()) {
    out << ":" << (step.has_value() ? std::to_string(step.value()) : "");
  }

  out << "]";

  return out.str();
}
