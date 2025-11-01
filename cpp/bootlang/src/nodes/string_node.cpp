#include "nodes/string_node.hpp"

StringNode::StringNode(size_t lineno, size_t col, std::string value)
    : Node(lineno, col, Node::Type::STRING), value(value) {}

std::string StringNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << '"' << value << '"';

    return out.str();
}