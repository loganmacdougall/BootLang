#pragma once

#include <vector>
#include "node.hpp"

typedef std::unique_ptr<class BlockNode> BlockNodePtr;

class BlockNode : public Node {
public:
    std::vector<NodePtr> stmts;

    BlockNode(uint32_t lineno, uint32_t col, std::vector<NodePtr>&& stmts);

    std::string toCode(int indent) const override;
};