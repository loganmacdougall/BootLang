#include "toplevel_context.hpp"

size_t TopLevelContext::emit(Instruction::Type type, size_t arg) {
    instructions.emplace_back(type, arg);
    return instructions.size() - 1;
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

size_t TopLevelContext::idConstant(const Value&& value) {
  size_t h = value.hash();

  auto it = constants_hash_map.find(h);
  if (it != constants_hash_map.end()) {
      for (auto idx : it->second) {
          if (value.equal(*constants[idx])) {
              return idx;
          }
      }
  }

  size_t index = constants.size();
  constants.push_back(std::make_shared<Value>(std::move(value)));
  constants_hash_map[h].push_back(index);
  return index;
}

size_t TopLevelContext::getConstantId(const Value& value) const {
    size_t h = value.hash();

    auto it = constants_hash_map.find(h);
    if (it != constants_hash_map.end()) {
        for (auto idx : it->second) {
            if (value.equal(*constants[idx])) {
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

  for (size_t i = 0; i < instructions.size(); i++) {
      const Instruction& inst = instructions[i];
      std::string inst_name = InstructionMetadata::GetInstance().GetInstructionName(inst.type);
      std::string space = std::string(NAME_SPACE - inst_name.size(), ' ');
      out << "  " << inst_name << space << inst.arg << std::endl;
  }

  return out.str();
}