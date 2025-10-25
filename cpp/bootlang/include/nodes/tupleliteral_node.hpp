#pragma once

#include <vector>
#include "node.hpp"

class TupleLiteralNode : public Node {
public:
    std::vector<NodePtr> elems;

    TupleLiteralNode(uint32_t lineno, uint32_t col, std::vector<NodePtr>&& elems);

    std::string toCode(int indent) const override;
};