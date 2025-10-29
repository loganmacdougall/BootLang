#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include "toplevel_context.hpp"
#include "instruction.hpp"
#include "value.hpp"

class FunctionContext : public Context {
public:
  std::vector<std::string> vars;
  std::unordered_map<std::string, size_t> vars_map;
  std::vector<std::string> freevars;
  std::unordered_map<std::string, size_t> freevars_map;
  std::vector<std::string> cellvars;
  std::unordered_map<std::string, size_t> cellvars_map;

  TopLevelContext& top_context;

  static const size_t NOT_FOUND = -1;

  FunctionContext(TopLevelContext& top_context);
  size_t idVar(const std::string& name);
  size_t getVarId(const std::string& name) const;
  size_t idFreeVar(const std::string& name);
  size_t getFreeVarId(const std::string& name) const;
  size_t idCellVar(const std::string& name);
  size_t getCellVarId(const std::string& name) const;

  virtual size_t idGlobal(const std::string& name) override;
  virtual size_t getGlobalId(const std::string& name) const override;
  virtual std::string toDisassembly() const override;
};