#pragma once

#include <vector>
#include "node.hpp"

class ListLiteralNode : public Node {
public:
    std::vector<NodePtr> elems;

    ListLiteralNode(uint32_t lineno, uint32_t col, std::vector<NodePtr>&& elems);

    std::string toCode(int indent) const override;
};