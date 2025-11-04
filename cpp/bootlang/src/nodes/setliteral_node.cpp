#include "nodes/setliteral_node.hpp"

SetLiteralNode::SetLiteralNode(size_t lineno, size_t col, std::vector<NodePtr>&& elems)
: Node(lineno, col, Node::Type::SET_LITERAL, true), elems(std::move(elems)) {
    for (auto &elem : elems) {
        if (!elem->constant) {
            constant = false;
            break;
        }
    }
}

std::string SetLiteralNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << '{';
    for (size_t i = 0; i < elems.size(); i++) {
        if (i > 0) out << ", ";
        out << elems[i]->toCode(indent);
    }
    out << '}';

    return out.str();
}