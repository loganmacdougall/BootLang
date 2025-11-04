#include "nodes/bool_node.hpp"

BoolNode::BoolNode(size_t lineno, size_t col, bool value)
    : Node(lineno, col, Node::Type::BOOL, true), value(value) {}

std::string BoolNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << (value ? "True" : "False");

    return out.str();
}