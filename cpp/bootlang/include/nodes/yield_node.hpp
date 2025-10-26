#pragma once

#include <optional>
#include "node.hpp"

class YieldNode : public Node {
  public:
    std::optional<NodePtr> value;

    YieldNode(uint32_t lineno, uint32_t col, std::optional<NodePtr>&& value);

    std::string toCode(int indent) const override;
};