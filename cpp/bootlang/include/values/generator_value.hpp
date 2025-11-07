#pragma once

#include "value.hpp"
#include "code_object.hpp"
#include "values/iterator_value.hpp"

class GeneratorValue : public Value {
  struct GeneratorIterState : public IteratorState {
    size_t ip;
    std::vector<Value::Ptr> stack;
    std::vector<Value::Ptr> locals;
    bool finished;
  };

  public:
    const CodeObject& code;

    GeneratorValue(const CodeObject& code);
    virtual Value::Ptr clone() const override;
    virtual std::string toString() const override;
};