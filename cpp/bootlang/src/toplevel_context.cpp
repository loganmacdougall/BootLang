#include "toplevel_context.hpp"

size_t TopLevelContext::emit(Instruction::Type type, size_t arg) {
    instructions.emplace_back(type, arg);
    return instructions.size() - 1;
}

Instruction TopLevelContext::burn() {
    Instruction inst = instructions.back();
    instructions.pop_back();
    return inst;
}

void TopLevelContext::patch(size_t index, Instruction::Type type, size_t arg) {
  instructions[index] = Instruction(type, arg);
}

size_t TopLevelContext::len() const {
  return instructions.size();
}

const Instruction& TopLevelContext::get(size_t index) const {
  return instructions[index];
}

size_t TopLevelContext::idConstant(const Value* value) {
  size_t h = value->hash();

  auto it = constants_hash_map.find(h);
  if (it != constants_hash_map.end()) {
      for (auto idx : it->second) {
          if (value->equal(*constants[idx])) {
              return idx;
          }
      }
  }

  Value::Ptr copy = std::make_shared<Value>(value->clone());

  size_t index = constants.size();
  constants.push_back(copy);
  constants_hash_map[h].push_back(index);
  return index;
}

size_t TopLevelContext::getConstantId(const Value* value) const {
    size_t h = value->hash();

    auto it = constants_hash_map.find(h);
    if (it != constants_hash_map.end()) {
        for (auto idx : it->second) {
            if (value->equal(*constants[idx])) {
                return idx;
            }
        }
    }

    return -1;
}

size_t TopLevelContext::idGlobal(const std::string& name) {
    auto it = global_map.find(name);
    if (it != global_map.end()) {
        return it->second;
    }

    size_t index = globals.size();
    globals.push_back(name);
    global_map[name] = index;
    return index;
}

size_t TopLevelContext::getGlobalId(const std::string& name) const {
    auto it = global_map.find(name);
    if (it != global_map.end()) {
        return it->second;
    } else {
        return -1;
    }
}

std::string TopLevelContext::toDissassembly() const {
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
            out << "(" << globals[inst.arg] << ")";
            break;
        case Instruction::Type::LOAD_CONST:
            out << "(" << constants[inst.arg].get() << ")";
            break;
        case Instruction::Type::BINARY_OP:
            temp = TokenMetadata::GetInstance().GetTokenString(static_cast<Token::Type>(inst.arg));
            out << "(" << temp << ")";
            break;
        default:
            break;
      };
      
      out << std::endl;
  }

  return out.str();
}