#include "nodes/dictliteral_node.hpp"

DictLiteralNode::DictLiteralNode(uint32_t lineno, uint32_t col, std::vector<DictPair>&& elems)
: Node(lineno, col, NodeType::n_DICT_LITERAL), elems(std::move(elems)) {}

std::string DictLiteralNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << '{';
    for (size_t i = 0; i < elems.size(); i++) {
        if (i > 0) out << ", ";
        out << elems[i].first->toCode(indent) << ": " << elems[i].second->toCode(indent);
    }
    out << '}';

    return out.str();
}