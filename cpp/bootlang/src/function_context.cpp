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

  if (parent) {
    return parent->idCellVar(name);
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
    emit(Instruction::LOAD_DEREF, free_id);
    return;
  }

  if (parent != nullptr) {
    free_id = parent->idCellVar(name);
    if (free_id != Context::NOT_FOUND) {
      size_t local_free_id = idFreeVar(name);
      emit(Instruction::LOAD_DEREF, local_free_id);
      return;
    }
  }
  
  size_t id = top_context->idVar(name);
  emit(Instruction::LOAD_GLOBAL, id);
}

void FunctionContext::storeIdentifier(const std::string& name) {
  size_t var_id = getVarId(name);
  if (var_id != Context::NOT_FOUND) {
    emit(Instruction::STORE_FAST, var_id);
    return;
  }

  size_t free_id = getFreeVarId(name);
  if (free_id != Context::NOT_FOUND) {
    emit(Instruction::STORE_DEREF, free_id);
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
  std::string temp;

  for (size_t i = 0; i < cellvars.size(); i++) {
      out << "  MAKE_CELL" << std::string(22, ' ') << i;
      out << " (" << cellvars[i] << ")" << std::endl;
  }

  if (!cellvars.empty()) {
    out << std::endl;
  }

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
            out << ((inst.arg < top_context->vars.size()) ? top_context->vars[inst.arg] : "???");
            out << ")";
            break;
        case Instruction::Type::LOAD_CONST:
        case Instruction::Type::LOAD_ATTR:
        case Instruction::Type::STORE_ATTR:
            out << "(";
            out << ((inst.arg < constants.size()) ? constants[inst.arg].get()->toCode() : "???");
            out << ")";
            break;
        case Instruction::Type::LOAD_FAST:
        case Instruction::Type::STORE_FAST:
            out << "(";
            out << ((inst.arg < vars.size()) ? vars[inst.arg] : "???");
            out << ")";
            break;
        case Instruction::Type::LOAD_DEREF:
        case Instruction::Type::STORE_DEREF:
            out << "(";
            out << ((inst.arg < freevars.size()) ? freevars[inst.arg] : "???");
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