#include "nodes/tupleliteral_node.hpp"

TupleLiteralNode::TupleLiteralNode(uint32_t lineno, uint32_t col, std::vector<NodePtr>&& elems)
: Node(lineno, col, NodeType::n_TUPLE_LITERAL), elems(std::move(elems)) {}

std::string TupleLiteralNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << '(';
    for (size_t i = 0; i < elems.size(); i++) {
        if (i > 0) out << ", ";
        out << elems[i]->toCode(indent);
    }
    out << ')';

    return out.str();
}