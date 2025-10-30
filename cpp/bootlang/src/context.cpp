#include "context.hpp"

size_t Context::emit(Instruction::Type type, size_t arg) {
    instructions.emplace_back(type, arg);
    return instructions.size() - 1;
}

Instruction Context::burn() {
    Instruction inst = instructions.back();
    instructions.pop_back();
    return inst;
}

void Context::patch(size_t index, Instruction::Type type, size_t arg) {
  instructions[index] = Instruction(type, arg);
}

size_t Context::len() const {
  return instructions.size();
}

const Instruction& Context::get(size_t index) const {
  return instructions[index];
}

size_t Context::idVar(const std::string& name) {
  auto it = vars_map.find(name);
  if (it != vars_map.end()) {
      return it->second;
  }

  size_t index = vars.size();
  vars.push_back(name);
  vars_map[name] = index;
  return index;
}

size_t Context::getVarId(const std::string& name) const {
  auto it = vars_map.find(name);
  if (it != vars_map.end()) {
      return it->second;
  }

  return -1;
}

size_t Context::idConstant(const Value* value) {
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

size_t Context::getConstantId(const Value* value) const {
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

void Context::loadIdentifier(const std::string& name) {
    (void)name;
    throw std::runtime_error("Base context doesn't support loadIdentifier");
}

void Context::storeIdentifier(const std::string& name) {
    (void)name;
    throw std::runtime_error("Base context doesn't support storeIdentifier");
}

std::string Context::toDisassembly() const {
    throw std::runtime_error("Base context doesn't support toDisassembly");
}
