#include "function_context.hpp"

FunctionContext::FunctionContext(TopLevelContext& top_context) 
: top_context(top_context) {}

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

size_t FunctionContext::idGlobal(const std::string& name) {
  return top_context.idGlobal(name);
}

size_t FunctionContext::getGlobalId(const std::string& name) const {
  return top_context.getGlobalId(name);
}

std::string FunctionContext::toDisassembly() const {
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