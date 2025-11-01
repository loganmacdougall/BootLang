#pragma once

#include <optional>
#include "node.hpp"

class ReturnNode : public Node {
public:
    std::optional<NodePtr> value;

    ReturnNode(size_t lineno, size_t col, std::optional<NodePtr>&& value);

    std::string toCode(int indent) const override;
};
