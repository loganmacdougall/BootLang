#include "nodes/assign_node.hpp"

AssignNode::AssignNode(size_t lineno, size_t col, NodePtr&& left, NodePtr&& right, Token::Type op)
    : Node(lineno, col, Node::Type::ASSIGN), left(std::move(left)), right(std::move(right)), op(op) {}

std::string AssignNode::toCode(int indent) const {
    (void)indent;
    std::ostringstream out;

    out << left->toCode(indent) << ' ';
    out << TokenMetadata::GetInstance().GetTokenString(op) << ' ';
    out << right->toCode(indent);

    return out.str();
}