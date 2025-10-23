#include "nodes/continue_node.hpp"

ContinueNode::ContinueNode(uint32_t lineno, uint32_t col)
    : Node(lineno, col, NodeType::n_CONTINUE) {}

std::string ContinueNode::toCode(int indent) const {
    (void)indent; 
    return "continue";
}