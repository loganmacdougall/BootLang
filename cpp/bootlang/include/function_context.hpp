#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include <sstream>
#include <string>
#include "toplevel_context.hpp"
#include "instruction.hpp"
#include "value.hpp"

class FunctionContext : public Context {
public:
  std::vector<std::string> freevars;
  std::unordered_map<std::string, size_t> freevars_map;
  std::vector<std::string> cellvars;
  std::unordered_map<std::string, size_t> cellvars_map;

  std::shared_ptr<TopLevelContext> top_context;
  std::shared_ptr<FunctionContext> parent;

  static const size_t NOT_FOUND = -1;

  FunctionContext(std::shared_ptr<TopLevelContext> top_context, std::shared_ptr<FunctionContext> parent);
  size_t idFreeVar(const std::string& name);
  size_t getFreeVarId(const std::string& name) const;
  size_t idCellVar(const std::string& name);
  size_t getCellVarId(const std::string& name) const;

  virtual Instruction loadIdentifierInstruction(const std::string& name) override;
  virtual Instruction storeIdentifierInstruction(const std::string& name) override;
  virtual std::string toDisassembly() const override;
};