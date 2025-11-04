#include "nodes/var_node.hpp"

VarNode::VarNode(size_t lineno, size_t col, std::string name)
    : Node(lineno, col, Node::Type::VAR, false), name(name) {}

std::string VarNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << name;

    return out.str();
}