#include "nodes/break_node.hpp"

BreakNode::BreakNode(uint32_t lineno, uint32_t col)
    : Node(lineno, col, Node::Type::BREAK) {}

std::string BreakNode::toCode(int indent) const {
    (void)indent; 
    return "break";
}