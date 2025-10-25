#include "nodes/bool_node.hpp"

BoolNode::BoolNode(uint32_t lineno, uint32_t col, bool value)
    : Node(lineno, col, Node::Type::BOOL), value(value) {}

std::string BoolNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << (value ? "True" : "False");

    return out.str();
}