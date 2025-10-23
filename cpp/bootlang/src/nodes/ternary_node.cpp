#include "nodes/ternary_node.hpp"

TernaryNode::TernaryNode(uint32_t lineno, uint32_t col, Node left, Node right, Node cond)
    : Node(lineno, col, NodeType::n_TERNARY), left(left), right(right), cond(cond) {}

std::string TernaryNode::toCode(int indent) const {
    (void)indent;
    std::ostringstream out;

    out << left.toCode(indent) << ' ';
    out << "if " << cond.toCode(indent) << ' ';
    out << "else " << right.toCode(indent);

    return out.str();
}