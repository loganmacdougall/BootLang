#include "nodes/for_node.hpp"

ForNode::ForNode(uint32_t lineno, uint32_t col, NodePtr&& target, NodePtr&& iterable, BlockNodePtr&& block)
: Node(lineno, col, Node::Type::FOR), target(std::move(target)), iterable(std::move(iterable)), block(std::move(block)) {}

std::string ForNode::toCode(int indent) const {
    std::ostringstream out;

    out << "for " << target->toCode(indent);
    out << " in " << iterable->toCode(indent) << ":\n";
    out << block->toCode(indent + 2);

    return out.str();
}