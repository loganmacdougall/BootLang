#include "nodes/setliteral_node.hpp"

SetLiteralNode::SetLiteralNode(uint32_t lineno, uint32_t col, std::vector<NodePtr>&& elems)
: Node(lineno, col, Node::Type::SET_LITERAL), elems(std::move(elems)) {}

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