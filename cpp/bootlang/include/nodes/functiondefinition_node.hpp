#pragma once

#include <vector>
#include "node.hpp"
#include "nodes/block_node.hpp"

class FunctionDefinitionNode : public Node {
public:
    std::string name;
    std::vector<std::string> args;
    BlockNodePtr block;

    FunctionDefinitionNode(uint32_t lineno, uint32_t col, std::string name, std::vector<std::string>&& args, BlockNodePtr&& block);

    std::string toCode(int indent) const override;
};