#include "nodes/while_node.hpp"

WhileNode::WhileNode(uint32_t lineno, uint32_t col, NodePtr&& cond, BlockNodePtr&& block)
: Node(lineno, col, NodeType::n_FLOAT), cond(std::move(cond)), block(std::move(block)) {}

std::string WhileNode::toCode(int indent) const {
    std::ostringstream out;

    out << "while " << cond->toCode(indent) << ":\n";
    out << block->toCode(indent + 2);

    return out.str();
}