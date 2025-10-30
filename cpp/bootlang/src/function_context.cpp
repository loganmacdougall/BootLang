#include "function_context.hpp"

FunctionContext::FunctionContext(std::shared_ptr<TopLevelContext> top_context, std::shared_ptr<FunctionContext> parent) 
: top_context(top_context), parent(parent) {}

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

  return Context::NOT_FOUND;
}

size_t FunctionContext::idCellVar(const std::string& name) {
  auto cell_it = cellvars_map.find(name);
  if (cell_it != cellvars_map.end()) {
    return cell_it->second;
  }

  auto var_it = vars_map.find(name);
  if (var_it != vars_map.end()) {  
    size_t index = cellvars.size();
    cellvars.push_back(name);
    cellvars_map[name] = index;
    return index;
  }

  return Context::NOT_FOUND;
}

size_t FunctionContext::getCellVarId(const std::string& name) const {
  auto it = cellvars_map.find(name);
  if (it != cellvars_map.end()) {
      return it->second;
  }

  return Context::NOT_FOUND;
}

void FunctionContext::loadIdentifier(const std::string& name) {
  size_t var_id = getVarId(name);
  if (var_id != Context::NOT_FOUND) {
    emit(Instruction::LOAD_FAST, var_id);
    return;
  }

  size_t free_id = getFreeVarId(name);
  if (free_id != Context::NOT_FOUND) {
    emit(Instruction::LOAD_DEFER, free_id);
    return;
  }

  if (parent != nullptr) {
    free_id = parent->idCellVar(name);
    if (free_id != Context::NOT_FOUND) {
      
      emit(Instruction::LOAD_DEFER, free_id);
    }
  }
  
  top_context->loadIdentifier(name);
}

void FunctionContext::storeIdentifier(const std::string& name) {
  size_t var_id = getVarId(name);
  if (var_id != Context::NOT_FOUND) {
    emit(Instruction::STORE_FAST, var_id);
    return;
  }

  size_t free_id = getFreeVarId(name);
  if (free_id != Context::NOT_FOUND) {
    emit(Instruction::STORE_DEFER, free_id);
    return;
  }

  size_t global_id = top_context->getVarId(name);
  if (global_id != Context::NOT_FOUND) {
    emit(Instruction::STORE_GLOBAL, global_id);
    return;
  }

  size_t new_id = idVar(name);
  emit(Instruction::STORE_FAST, new_id);
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
            out << "(" <<  top_context->vars[inst.arg] << ")";
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