#include "nodes/int_node.hpp"

IntNode::IntNode(uint32_t lineno, uint32_t col, long value)
    : Node(lineno, col, NodeType::n_INT), value(value) {}

std::string IntNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << value;

    return out.str();
}