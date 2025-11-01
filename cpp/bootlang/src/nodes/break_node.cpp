#include "nodes/break_node.hpp"

BreakNode::BreakNode(size_t lineno, size_t col)
    : Node(lineno, col, Node::Type::BREAK) {}

std::string BreakNode::toCode(int indent) const {
    (void)indent; 
    return "break";
}