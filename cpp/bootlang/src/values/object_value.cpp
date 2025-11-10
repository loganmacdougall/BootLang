#include "values/object_value.hpp"

ObjectValue::ObjectValue(const ClassValue* base)
: Value(Value::Type::OBJECT), base(base) {}

Value::Ptr ObjectValue::getProperty(std::string name) {

}

Value::Ptr ObjectValue::setProperty(std::string name, Value::Ptr value) {

}


std::string ObjectValue::toString() const {

}
