#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include "context.hpp"
#include "instruction.hpp"
#include "value.hpp"
#include "token.hpp"

class TopLevelContext : public Context {
public:
  std::vector<std::string> globals;
  std::unordered_map<std::string, size_t> global_map;

  virtual size_t idGlobal(const std::string& name) override;
  virtual size_t getGlobalId(const std::string& name) const override;
  virtual std::string toDisassembly() const override;
};