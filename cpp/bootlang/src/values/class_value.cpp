  #include "values/class_value.hpp"
  #include "values/object_value.hpp"
  
  ClassValue::ClassValue(std::shared_ptr<FunctionValue> block) 
  : Value(Value::Type::CLASS), block(std::move(block)) {}

  Value::Ptr ClassValue::call(Value::CallableInfo& info) {
    auto object = std::make_shared<ObjectValue>(this);
    
  }

  std::size_t ClassValue::hash() const {

  }

  bool ClassValue::equal(const Value& other) const {

  }

  Value::Ptr ClassValue::clone() const {

  }

  std::string ClassValue::toString() const {

  }
