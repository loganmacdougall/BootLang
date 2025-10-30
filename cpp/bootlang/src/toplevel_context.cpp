#include "toplevel_context.hpp"

void TopLevelContext::loadIdentifier(const std::string& name) {
    size_t id = idVar(name);
    emit(Instruction::LOAD_GLOBAL, id);
}

void TopLevelContext::storeIdentifier(const std::string& name) {
    size_t id = idVar(name);
    emit(Instruction::STORE_GLOBAL, id);
}

std::string TopLevelContext::toDisassembly() const {
  std::ostringstream out;

  const size_t NAME_SPACE = 32;
  std::string temp;

  for (size_t i = 0; i < instructions.size(); i++) {
      const Instruction& inst = instructions[i];
      std::string inst_name = InstructionMetadata::GetInstance().GetInstructionName(inst.type);
      std::string arg_string = std::to_string(inst.arg);
      size_t space_size = NAME_SPACE - inst_name.size() - arg_string.size();
      space_size = space_size >= 1 ? space_size : 1;
      std::string space = std::string(space_size, ' ');
      out << "  " << inst_name << space << inst.arg << ' ';
      
      switch (inst.type) {
        case Instruction::Type::LOAD_INT:
            out << "(" << inst.arg << ")";
            break;
        case Instruction::Type::LOAD_GLOBAL:
        case Instruction::Type::STORE_GLOBAL:
            out << "(";
            out << (vars.size() < inst.arg) ? vars[inst.arg] : "???";
            out << ")";
            break;
        case Instruction::Type::LOAD_CONST:
            out << "(";
            out << (constants.size() < inst.arg) ? constants[inst.arg].get()->toCode() : "???";
            out << ")";
            break;
        case Instruction::Type::BINARY_OP:
        case Instruction::Type::UNARY_OP:
            temp = TokenMetadata::GetInstance().GetTokenString(static_cast<Token::Type>(inst.arg));
            out << "(" << temp << ")";
            break;
        default:
            break;
      };
      
      out << "\n";
  }

  return out.str();
}