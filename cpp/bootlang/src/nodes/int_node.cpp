#include "nodes/int_node.hpp"

IntNode::IntNode(size_t lineno, size_t col, long value)
    : Node(lineno, col, Node::Type::INT), value(value) {}

std::string IntNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << value;

    return out.str();
}