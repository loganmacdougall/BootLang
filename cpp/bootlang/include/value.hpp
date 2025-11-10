#pragma once

#include <string>
#include <array>
#include <vector>
#include <functional>
#include <stdexcept>
#include <memory>
#include <any>

class Value {
  public:
    typedef std::shared_ptr<Value> Ptr;
    typedef std::function<Value::Ptr(Value::Ptr func, Value::Ptr self, std::vector<Value::Ptr>&& args)> VMCallback;

    enum Type {
        NONE,
        INT,
        FLOAT,
        CHAR,
        STRING,
        BOOL,
        FUNCTION,
        BUILTIN_FUNCTION,
        BUILTIN_GENERATOR,
        CLASS,
        OBJECT,
        LIST,
        SLICE,
        DICT,
        SET,
        TUPLE,
        GENERATOR,
        ITERATOR
    };

    static const size_t VALUE_COUNT = static_cast<size_t>(Type::ITERATOR) + 1;
    static const size_t LIST_DISPLAY_HALF_WIDTH = 128;

    Type type;

    struct IteratorState {
        bool finished = false;
        virtual ~IteratorState() = default;
    };

    struct CallableInfo {
      Value::Ptr func_pointer;
      Value::Ptr self;
      std::vector<Value::Ptr> args;
      VMCallback vm_call;

      CallableInfo(
        Value::Ptr func_pointer,
        Value::Ptr self,
        std::vector<Value::Ptr>&& args,
        VMCallback vm_call
      );
    };

    Value(Type type);
    virtual bool toBool() const { return true; }
    virtual Value::Ptr call(CallableInfo& info);
    virtual std::size_t hash() const;
    virtual bool equal(const Value& other) const;
    virtual Value::Ptr next(std::shared_ptr<Value::IteratorState> base_state) const;
    virtual std::shared_ptr<Value::IteratorState> toIter() const;
    virtual size_t len() const;
    virtual Value::Ptr clone() const;
    virtual std::string toString() const;
    const std::string& typeString() const;
    static Value::Ptr copy(Value::Ptr value) { return value; }

    template<typename T>
    static inline std::shared_ptr<T> toDerived(Value::Ptr value) {
        return static_pointer_cast<T>(value);
    }

    template<typename T>
    static inline std::shared_ptr<Value> toBase(std::shared_ptr<T> value) {
        return static_pointer_cast<Value>(value);
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

class ValueMetadata {
  public:
    typedef std::pair<Value::Type, std::string> RegexPair;
    
  public:
    static const ValueMetadata& GetInstance();
    const std::string& GetValueName(Value::Type token) const;
    
  private:
    ValueMetadata() = default;

    const std::array<std::string, Value::VALUE_COUNT> value_names{
        "NONE", "INT", "FLOAT", "CHAR", "STRING", "BOOL",
        "FUNCTION", "BUILTIN_FUNCTION", "BUILTIN_GENERATOR",
        "CLASS", "OBJECT",
        "LIST", "SLICE", "DICT", "SET", "TUPLE",
        "GENERATOR", "ITERATOR"
    };
    
  public:
    ValueMetadata(ValueMetadata const&) = delete;
    void operator=(ValueMetadata const&) = delete;
};