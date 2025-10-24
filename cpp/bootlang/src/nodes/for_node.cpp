#include "nodes/for_node.hpp"

ForNode::ForNode(uint32_t lineno, uint32_t col, std::vector<std::string>&& args, NodePtr&& iterable, BlockNodePtr&& block)
: Node(lineno, col, NodeType::n_FOR), args(std::move(args)), iterable(std::move(iterable)), block(std::move(block)) {}

std::string ForNode::toCode(int indent) const {
    std::ostringstream out;

    out << "for ";
    for (size_t i = 0; i < args.size(); i++) {
        if (i > 0) out << ", ";
        out << args[i];
    }
    out << " in " << iterable->toCode(indent) << ":\n";
    out << block->toCode(indent + 2);

    return out.str();
}