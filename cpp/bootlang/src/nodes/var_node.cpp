#include "nodes/var_node.hpp"

VarNode::VarNode(uint32_t lineno, uint32_t col, std::string name)
    : Node(lineno, col, NodeType::n_VAR), name(name) {}

std::string VarNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << name;

    return out.str();
}