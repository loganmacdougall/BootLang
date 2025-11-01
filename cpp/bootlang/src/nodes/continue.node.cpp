#include "nodes/continue_node.hpp"

ContinueNode::ContinueNode(size_t lineno, size_t col)
    : Node(lineno, col, Node::Type::CONTINUE) {}

std::string ContinueNode::toCode(int indent) const {
    (void)indent; 
    return "continue";
}