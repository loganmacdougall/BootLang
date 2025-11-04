#include "nodes/listliteral_node.hpp"

ListLiteralNode::ListLiteralNode(size_t lineno, size_t col, std::vector<NodePtr>&& elems)
: Node(lineno, col, Node::Type::LIST_LITERAL, true), elems(std::move(elems)) {
    for (auto &elem : elems) {
        if (!elem->constant) {
            constant = false;
            break;
        }
    }
}

std::string ListLiteralNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << '[';
    for (size_t i = 0; i < elems.size(); i++) {
        if (i > 0) out << ", ";
        out << elems[i]->toCode(indent);
    }
    out << ']';

    return out.str();
}