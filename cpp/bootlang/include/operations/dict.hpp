#include <stdexcept>
#include "value.hpp"
#include "values/map_value.hpp"
#include "values/tuple_value.hpp"
#include "values/builtin_generator_value.hpp"

Value::Ptr dict_items(Value::CallableInfo& info);
std::shared_ptr<Value::IteratorState> dict_items_init(const Value::CallableInfo& info);
Value::Ptr dict_items_next(std::shared_ptr<Value::IteratorState> base_state);

class DictItemsIterState : public Value::IteratorState {
public:  
  std::map<size_t, Value::Ptr>::const_iterator it;
  std::shared_ptr<MapValue> dict = nullptr;
};