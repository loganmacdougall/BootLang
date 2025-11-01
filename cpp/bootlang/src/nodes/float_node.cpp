#include "nodes/float_node.hpp"

FloatNode::FloatNode(size_t lineno, size_t col, double value)
    : Node(lineno, col, Node::Type::FLOAT), value(value) {}

std::string FloatNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << value;

    return out.str();
}