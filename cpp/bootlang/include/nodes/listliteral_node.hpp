#pragma once

#include <vector>
#include "node.hpp"

class ListLiteralNode : public Node {
public:
    std::vector<NodePtr> elems;

    ListLiteralNode(size_t lineno, size_t col, std::vector<NodePtr>&& elems);

    std::string toCode(int indent) const override;
};