#pragma once

#include <string>
#include <stdexcept>
#include <memory>
#include <any>

class Value {
  public:
    typedef std::shared_ptr<Value> Ptr;

    enum Type {
        NONE,
        INT,
        FLOAT,
        CHAR,
        STRING,
        BOOL,
        FUNCTION,
        LIST,
        MAP,
        SET,
        TUPLE,
        GENERATOR,
        ITERATOR
    };

    static const size_t LIST_DISPLAY_HALF_WIDTH = 16;

    Type type;

    struct IteratorState {
      virtual ~IteratorState() = default;
    };

    Value(Type type);
    virtual bool isHashable() const { return false; }
    virtual bool isIterable() const { return false; }
    virtual bool hasLength() const { return false; }
    virtual std::size_t hash() const;
    virtual bool equal(const Value& other) const;
    virtual Value::Ptr nextFromIter(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> iterInitialState() const;
    virtual size_t len() const;
    virtual Value::Ptr clone() const;
    virtual std::string toCode() const;

    template<typename T>
    static inline std::shared_ptr<T> toDerived(Value::Ptr value) {
        return static_pointer_cast<std::shared_ptr<T>>(value);
    }

    template<typename T>
    static inline std::shared_ptr<Value> toBase(std::shared_ptr<T> value) {
        return static_pointer_cast<Value::Ptr>(value);
    }

    template<typename T>
    static inline const T* toDerived(const Value* value) {
        return static_cast<const T*>(value);
    }

    template<typename T>
    static inline const Value* toBase(const T* value) {
        return static_cast<const Value*>(value);
    }

    virtual ~Value() = default;
};