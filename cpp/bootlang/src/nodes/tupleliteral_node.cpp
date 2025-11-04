#include "nodes/tupleliteral_node.hpp"

TupleLiteralNode::TupleLiteralNode(size_t lineno, size_t col, std::vector<NodePtr>&& elems)
: Node(lineno, col, Node::Type::TUPLE_LITERAL, true), elems(std::move(elems)) {
    for (auto &elem : elems) {
        if (!elem->constant) {
            constant = false;
            break;
        }
    }
}

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