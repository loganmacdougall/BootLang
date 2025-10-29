#include "function_context.hpp"

FunctionContext::FunctionContext(TopLevelContext& top_context) 
: top_context(top_context) {}

size_t FunctionContext::emit(Instruction::Type type, size_t arg) {
  instructions.emplace_back(type, arg);
  return instructions.size() - 1;
}

void FunctionContext::patch(size_t index, Instruction::Type type, size_t arg) {
  instructions[index] = Instruction(type, arg);
}

size_t FunctionContext::len() const {
  return instructions.size();
}

const Instruction& FunctionContext::get(size_t index) const {
  return instructions[index];
}

size_t FunctionContext::idConstant(const Value* value) {
  size_t h = value->hash();

  auto it = constants_hash_map.find(h);
  if (it != constants_hash_map.end()) {
      for (auto idx : it->second) {
          if (value->equal(*constants[idx])) {
              return idx;
          }
      }
  }

  size_t index = constants.size();
  constants.push_back(value->clone());
  constants_hash_map[h].push_back(index);
  return index;
}

size_t FunctionContext::getConstantId(const Value* value) const {
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

size_t FunctionContext::idVar(const std::string& name) {
  auto it = vars_map.find(name);
  if (it != vars_map.end()) {
      return it->second;
  }

  size_t index = vars.size();
  vars.push_back(name);
  vars_map[name] = index;
  return index;
}

size_t FunctionContext::getVarId(const std::string& name) const {
  auto it = vars_map.find(name);
  if (it != vars_map.end()) {
      return it->second;
  }

  return -1;
}

size_t FunctionContext::idFreeVar(const std::string& name) {
  auto it = freevars_map.find(name);
  if (it != freevars_map.end()) {
      return it->second;
  }

  size_t index = freevars.size();
  freevars.push_back(name);
  freevars_map[name] = index;
  return index;
}

size_t FunctionContext::getFreeVarId(const std::string& name) const {
  auto it = freevars_map.find(name);
  if (it != freevars_map.end()) {
      return it->second;
  }

  return -1;
}

size_t FunctionContext::idCellVar(const std::string& name) {
  auto it = cellvars_map.find(name);
  if (it != cellvars_map.end()) {
      return it->second;
  }

  size_t index = cellvars.size();
  cellvars.push_back(name);
  cellvars_map[name] = index;
  return index;
}

size_t FunctionContext::getCellVarId(const std::string& name) const {
  auto it = cellvars_map.find(name);
  if (it != cellvars_map.end()) {
      return it->second;
  }

  return -1;
}

std::string FunctionContext::toDissassembly() const {
  std::ostringstream out;

  const size_t NAME_SPACE = 32;

  for (size_t i = 0; i < instructions.size(); i++) {
      const Instruction& inst = instructions[i];
      std::string inst_name = InstructionMetadata::GetInstance().GetInstructionName(inst.type);
      std::string space = std::string(NAME_SPACE - inst_name.size(), ' ');
      out << "  " << inst_name << space << inst.arg << ' ';
      
      switch (inst.type) {
        case Instruction::Type::LOAD_INT:
            out << "(" << inst.arg << ")";
            break;
        case Instruction::Type::LOAD_GLOBAL:
        case Instruction::Type::STORE_GLOBAL:
            out << "(" <<  top_context.globals[inst.arg] << ")";
            break;
        case Instruction::Type::LOAD_CONST:
            out << "(" << constants[inst.arg] << ")";
            break;
        default:
            break;
      };

      out << std::endl;
  }


  return out.str();
}