#pragma once

#include <vector>
#include "node.hpp"
#include "nodes/block_node.hpp"

class FunctionDefinitionNode : public Node {
public:
    std::string name;
    std::vector<std::string> args;
    BlockNodePtr block;
    std::string doc;

    FunctionDefinitionNode(size_t lineno, size_t col, std::string name, std::vector<std::string>&& args, BlockNodePtr&& block, std::string doc);

    std::string toCode(int indent) const override;
};