#include "nodes/index_node.hpp"

IndexNode::IndexNode(uint32_t lineno, uint32_t col, Node left, Node right) 
: Node(lineno, col, NodeType::n_INDEX), left(left), right(right) {}

std::string IndexNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << left.toCode(indent) << '[' << right.toCode(indent) << ']';

    return out.str();
}