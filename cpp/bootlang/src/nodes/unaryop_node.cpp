#include "nodes/unaryop_node.hpp"

UnaryOpNode::UnaryOpNode(uint32_t lineno, uint32_t col, NodePtr&& right, Token::Type op)
    : Node(lineno, col, Node::Type::UNARY_OP), right(std::move(right)), op(op) {}

std::string UnaryOpNode::toCode(int indent) const {
    (void)indent;
    std::ostringstream out;

    out << '(';
    out << TokenMetadata::GetInstance().GetTokenString(op) << ' ';
    out << right->toCode(indent) << ')';

    return out.str();
}