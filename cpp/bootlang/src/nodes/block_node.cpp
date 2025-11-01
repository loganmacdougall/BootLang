#include "nodes/block_node.hpp"

BlockNode::BlockNode(size_t lineno, size_t col, std::vector<NodePtr>&& stmts)
    : Node(lineno, col, Node::Type::BLOCK), stmts(std::move(stmts)) {}

std::string BlockNode::toCode(int indent) const {
    std::ostringstream out;

    std::string indent_string = "";
    for (int i = 0; i < indent; i++) indent_string += ' ';

    for (size_t i = 0; i < stmts.size(); i++) {
        if (i > 0) out << '\n';
        out << indent_string << stmts[i]->toCode(indent);
    }

    return out.str();
}