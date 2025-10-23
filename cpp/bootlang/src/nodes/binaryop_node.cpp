#include "nodes/binaryop_node.hpp"

BinaryOpNode::BinaryOpNode(uint32_t lineno, uint32_t col, Node left, Node right, TokenType op)
    : Node(lineno, col, NodeType::n_BINARY_OP), left(left), right(right), op(op) {}

std::string BinaryOpNode::toCode(int indent) const {
    (void)indent;
    std::ostringstream out;

    out << '(' << left.toCode(indent) << ' ';
    out << TokenMetadata::GetInstance().GetTokenString(op) << ' ';
    out << right.toCode(indent) << ')';

    return out.str();
}