#include "nodes/return_node.hpp"

ReturnNode::ReturnNode(size_t lineno, size_t col, std::optional<NodePtr>&& value)
    : Node(lineno, col, Node::Type::RETURN), value(std::move(value)) {}

std::string ReturnNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << "return";
    if (value) {
        out << ' ' << value.value()->toCode(indent);
    }

    return out.str();
}