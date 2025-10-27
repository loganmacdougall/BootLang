#pragma once

#include <vector>
#include <cstdint>
#include <array>
#include <string>
#include <utility>

class Instruction {
  public:
  enum Type {
      LOAD_CONST,
      LOAD_GLOBAL,
      LOAD_VAR,
      LOAD_ATTR,
      LOAD_INDEX,
      LOAD_DEFER,
      STORE_GLOBAL,
      STORE_VAR,
      STORE_ATTR,
      STORE_INDEX,
      STORE_SLICE,
      STORE_DEFER,
      BINARY_OP,
      UNARY_OP,
      UNPACK_SEQUENCE,
      JUMP,
      JUMP_BACKWARDS,
      JUMP_IF_FALSE,
      CALL_FUNCTION,
      BUILD_TUPLE,
      BUILD_LIST,
      BUILD_MAP,
      BUILD_SET,
      BUILD_SLICE,
      MAKE_FUNCTION,
      MAKE_CLOSURE,
      MAKE_CELL,
      COPY_FREEVARS,
      PUSH_NULL,
      PAUSE,
      RETURN,
  };
  
  static const size_t INSTRUCTION_COUNT = static_cast<size_t>(Type::RETURN) + 1;
  
  public:
    Instruction::Type type;
    size_t arg;

    Instruction(Instruction::Type type, size_t arg);
};

class InstructionMetadata {
  public:
    typedef std::pair<Instruction::Type, std::string> RegexPair;
    
  public:
    static const InstructionMetadata& GetInstance();
    const std::string& GetInstructionName(Instruction::Type token) const;
    
  private:
    InstructionMetadata() = default;

    const std::array<std::string, Instruction::INSTRUCTION_COUNT> instruction_names{
      "LOAD_CONST", "LOAD_GLOBAL", "LOAD_VAR", "LOAD_ATTR", "LOAD_INDEX", "LOAD_DEFER",
      "STORE_GLOBAL", "STORE_VAR", "STORE_ATTR", "STORE_INDEX", "STORE_SLICE", "STORE_DEFER",
      "BINARY_OP", "UNARY_OP", "UNPACK_SEQUENCE",
      "JUMP", "JUMP_BACKWARDS", "JUMP_IF_FALSE", "CALL_FUNCTION",
      "BUILD_TUPLE", "BUILD_LIST", "BUILD_MAP", "BUILD_SET", "BUILD_SLICE",
      "MAKE_FUNCTION", "MAKE_CLOSURE", "MAKE_CELL", "COPY_FREEVARS",
      "PUSH_NULL","PAUSE", "RETURN"
    };
    
  public:
    InstructionMetadata(InstructionMetadata const&) = delete;
    void operator=(InstructionMetadata const&) = delete;
};