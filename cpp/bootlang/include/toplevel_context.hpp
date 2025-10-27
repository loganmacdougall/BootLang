#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include "instruction.hpp"
#include "value.hpp"
#include "token.hpp"

class TopLevelContext {
public:
  std::vector<Instruction> instructions;
  std::vector<size_t> line_starts;
  std::vector<std::string> globals;
  std::unordered_map<std::string, size_t> global_map;
  std::vector<Value::Ptr> constants;
  std::unordered_map<size_t, std::vector<size_t>> constants_hash_map;

    static const size_t NOT_FOUND = -1;

    TopLevelContext() = default;
    size_t emit(Instruction::Type type, size_t arg = 0);
    Instruction burn();
    void patch(size_t index, Instruction::Type type, size_t arg = 0);
    size_t len() const;
    const Instruction& get(size_t index) const;
    size_t idConstant(const Value* value);
    size_t getConstantId(const Value* value) const;
    size_t idGlobal(const std::string& name);
    size_t getGlobalId(const std::string& name) const;

    std::string toDissassembly() const;
};