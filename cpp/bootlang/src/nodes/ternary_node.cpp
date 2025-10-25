#include "nodes/ternary_node.hpp"

TernaryNode::TernaryNode(uint32_t lineno, uint32_t col, NodePtr&& left, NodePtr&& right, NodePtr&& cond)
    : Node(lineno, col, Node::Type::TERNARY), left(std::move(left)), right(std::move(right)), cond(std::move(cond)) {}

std::string TernaryNode::toCode(int indent) const {
    (void)indent;
    std::ostringstream out;

    out << left->toCode(indent) << ' ';
    out << "if " << cond->toCode(indent) << ' ';
    out << "else " << right->toCode(indent);

    return out.str();
}