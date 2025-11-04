#include "nodes/listliteral_node.hpp"

ListLiteralNode::ListLiteralNode(size_t lineno, size_t col, std::vector<NodePtr>&& elems)
: Node(lineno, col, Node::Type::LIST_LITERAL), elems(std::move(elems)) {}

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