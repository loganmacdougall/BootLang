#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include "toplevel_context.hpp"
#include "instruction.hpp"
#include "value.hpp"

class FunctionContext {
public:
  std::vector<Instruction> instructions;
  std::vector<size_t> line_starts;

  std::vector<Value::Ptr> constants;
  std::unordered_map<size_t, std::vector<size_t>> constants_hash_map;
  std::vector<std::string> vars;
  std::unordered_map<std::string, size_t> vars_map;
  std::vector<std::string> freevars;
  std::unordered_map<std::string, size_t> freevars_map;
  std::vector<std::string> cellvars;
  std::unordered_map<std::string, size_t> cellvars_map;

  TopLevelContext& top_context;

  static const size_t NOT_FOUND = -1;

  FunctionContext(TopLevelContext& top_context);
  size_t emit(Instruction::Type type, size_t arg = 0);
  void patch(size_t index, Instruction::Type type, size_t arg = 0);
  size_t len() const;
  const Instruction& get(size_t index) const;
  size_t idConstant(const Value* value);
  size_t getConstantId(const Value* value) const;
  size_t idVar(const std::string& name);
  size_t getVarId(const std::string& name) const;
  size_t idFreeVar(const std::string& name);
  size_t getFreeVarId(const std::string& name) const;
  size_t idCellVar(const std::string& name);
  size_t getCellVarId(const std::string& name) const;

  std::string toDissassembly() const;
};