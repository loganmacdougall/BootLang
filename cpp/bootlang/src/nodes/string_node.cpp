#include "nodes/string_node.hpp"

StringNode::StringNode(uint32_t lineno, uint32_t col, std::string value)
    : Node(lineno, col, NodeType::n_STRING), value(value) {}

std::string StringNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << '"' << value << '"';

    return out.str();
}