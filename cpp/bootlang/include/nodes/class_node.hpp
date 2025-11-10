#pragma once

#include "node.hpp"
#include "nodes/block_node.hpp"

class ClassNode : public Node {
public:
    std::string name;
    BlockNodePtr block;
    std::string doc;

    ClassNode(size_t lineno, size_t col, std::string name, BlockNodePtr&& block, std::string doc = "");

    std::string toCode(int indent) const override;
};