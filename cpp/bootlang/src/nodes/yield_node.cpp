#include "nodes/yield_node.hpp"

YieldNode::YieldNode(uint32_t lineno, uint32_t col, std::optional<NodePtr>&& value)
    : Node(lineno, col, Node::Type::YIELD), value(std::move(value)) {}

std::string YieldNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << "yield";
    if (value) {
        out << ' ' << value.value()->toCode(indent);
    }

    return out.str();
}