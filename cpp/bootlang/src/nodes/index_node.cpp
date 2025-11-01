#include "nodes/index_node.hpp"

IndexNode::IndexNode(size_t lineno, size_t col, NodePtr&& left, NodePtr&& right) 
: Node(lineno, col, Node::Type::INDEX), left(std::move(left)), right(std::move(right)) {}

std::string IndexNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << left->toCode(indent) << '[' << right->toCode(indent) << ']';

    return out.str();
}