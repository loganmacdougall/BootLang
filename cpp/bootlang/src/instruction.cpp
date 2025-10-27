#include "instruction.hpp"

Instruction::Instruction(Type type, size_t arg) : type(type), arg(arg) {}

const InstructionMetadata& InstructionMetadata::GetInstance() {
  static InstructionMetadata instance;
  return instance;
}

const std::string& InstructionMetadata::GetInstructionName(Instruction::Type token) const {
  return instruction_names[static_cast<size_t>(token)];
}