#pragma once

#include <vector>
#include <cstdint>

class Instruction {
  public:
  enum Type {
      LOAD_CONST,
      LOAD_GLOBAL,
      LOAD_VAR,
      LOAD_ATTR,
      LOAD_DEFER,
      STORE_GLOBAL,
      STORE_VAR,
      STORE_ATTR,
      STORE_DEFER,
      BINARY_OP,
      UNARY_OP,
      JUMP_IF_FALSE,
      JUMP,
      CALL_FUNCTION,
      BUILD_TUPLE,
      BUILD_LIST,
      BUILD_MAP,
      BUILD_SET,
      MAKE_FUNCTION,
      MAKE_CLOSURE,
      MAKE_CELL,
      COPY_FREEVARS,
      PAUSE,
      RETURN_CONST,
      RETURN_VALUE
  };
  
  public:
    Instruction::Type type;
    uint8_t arg;

    Instruction(Instruction::Type type, uint8_t arg);
};