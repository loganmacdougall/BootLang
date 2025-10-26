#pragma once

#include <stdexcept>
#include <memory>

class Value {
  public:
    typedef std::shared_ptr<Value> Ptr;

    enum Type {
        NONE,
        INT,
        FLOAT,
        STRING,
        BOOL,
        FUNCTION,
        LIST,
        MAP,
        SET,
        TUPLE,
        GENERATOR
    };

    Type type;

    Value(Type type);
    virtual bool isHashable() const { return false; }
    virtual std::size_t hash() const = 0;
    virtual bool equal(const Value& other) const;
    virtual ~Value() = default;
};