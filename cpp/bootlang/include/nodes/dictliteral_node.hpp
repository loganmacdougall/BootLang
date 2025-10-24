#pragma once

#include <vector>
#include "node.hpp"

typedef std::pair<NodePtr, NodePtr> DictPair;

class DictLiteralNode : public Node {
public:
    std::vector<DictPair> elems;

    DictLiteralNode(uint32_t lineno, uint32_t col, std::vector<DictPair>&& elems);

    std::string toCode(int indent) const override;
};