#include "nodes/setliteral_node.hpp"

SetLiteralNode::SetLiteralNode(uint32_t lineno, uint32_t col, std::vector<Node> elems)
: Node(lineno, col, NodeType::n_SET_LITERAL), elems(std::move(elems)) {}

std::string SetLiteralNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << '{';
    for (size_t i = 0; i < elems.size(); i++) {
        if (i > 0) out << ", ";
        out << elems[i].toCode(indent);
    }
    out << '}';

    return out.str();
}