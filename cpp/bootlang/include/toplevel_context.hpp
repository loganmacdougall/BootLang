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

  virtual inline bool topLevel() override { return true; }
  virtual void loadIdentifier(const std::string& name) override;
  virtual void storeIdentifier(const std::string& name) override;
  virtual std::string toDisassembly() const override;
};