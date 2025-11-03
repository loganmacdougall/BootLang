#include "toplevel_context.hpp"

size_t TopLevelContext::idBuiltin(std::string name) {
    auto it = builtins_map.find(name);
    if (it != builtins_map.end()) {
        return it->second;
    }

    size_t index = builtins.size();
    builtins.push_back(name);
    builtins_map[name] = index;
    return index;
}

size_t TopLevelContext::getBuiltinId(std::string name) const {
    auto it = builtins_map.find(name);
    if (it == builtins_map.end()) {
        return Context::NOT_FOUND;
    } else {
        return it->second;
    }
}

Instruction TopLevelContext::loadIdentifierInstruction(const std::string& name) {
    size_t id = getNameId(name);

    if (id != Context::NOT_FOUND) {
        return Instruction(Instruction::LOAD_GLOBAL, id);
    } else {
        size_t builtin_id = idBuiltin(name);
        return Instruction(Instruction::LOAD_BUILTIN, builtin_id);
    }
}

Instruction TopLevelContext::storeIdentifierInstruction(const std::string& name) {
    size_t id = idName(name);
    return Instruction(Instruction::STORE_GLOBAL, id);
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
        case Instruction::Type::LOAD_ATTR:
        case Instruction::Type::STORE_ATTR:
            out << "(";
            out << ((inst.arg < names.size()) ? names[inst.arg] : "???");
            out << ")";
            break;
        case Instruction::Type::LOAD_BUILTIN:
            out << "(";
            out << ((inst.arg < builtins.size()) ? builtins[inst.arg] : "???");
            out << ")";
            break;
        case Instruction::Type::LOAD_CONST:
            out << "(";
            out << ((inst.arg < constants.size()) ? constants[inst.arg].get()->toString() : "???");
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