#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include "instruction.hpp"
#include "value.hpp"
#include "token.hpp"

class Context {
public:
  std::vector<Instruction> instructions;
  std::vector<size_t> line_starts;
  std::vector<std::string> vars;
  std::unordered_map<std::string, size_t> vars_map;
  std::vector<Value::Ptr> constants;
  std::unordered_map<size_t, std::vector<size_t>> constants_hash_map;

  static const size_t NOT_FOUND = -1;

  size_t emit(Instruction::Type type, size_t arg = 0);
  Instruction burn();
  void patch(size_t index, Instruction::Type type, size_t arg = 0);
  size_t len() const;
  const Instruction& get(size_t index) const;
  size_t idVar(const std::string& name);
  size_t getVarId(const std::string& name) const;
  size_t idConstant(const Value* value);
  size_t getConstantId(const Value* value) const;
  Value::Ptr getConstant(size_t id);
  virtual void loadIdentifier(const std::string& name);
  virtual void storeIdentifier(const std::string& name);
  virtual inline bool topLevel() { return false; }
  virtual std::string toDisassembly() const;
};