#pragma once

#include <map>
#include <vector>
#include <sstream>
#include "value.hpp"
#include "values/class_value.hpp"

class ObjectValue : public Value {
  public:
    std::map<std::string, Value::Ptr> properties;
    const ClassValue* base;

    ObjectValue(const ClassValue* base);
    Value::Ptr getProperty(std::string name);
    Value::Ptr setProperty(std::string name, Value::Ptr value);

    virtual std::string toString() const override;
};