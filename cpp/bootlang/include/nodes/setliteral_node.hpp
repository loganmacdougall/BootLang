#pragma once

#include <vector>
#include "node.hpp"

class SetLiteralNode : public Node {
public:
    std::vector<NodePtr> elems;

    SetLiteralNode(size_t lineno, size_t col, std::vector<NodePtr>&& elems);

    std::string toCode(int indent) const override;
};