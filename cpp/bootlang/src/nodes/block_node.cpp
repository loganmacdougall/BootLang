#include "nodes/block_node.hpp"

BlockNode::BlockNode(uint32_t lineno, uint32_t col, std::vector<Node> stmts)
    : Node(lineno, col, NodeType::n_BLOCK), stmts(std::move(stmts)) {}

std::string BlockNode::toCode(int indent) const {
    std::ostringstream out;

    int block_indent = indent = 2;

    std::string indent_string = "";
    for (int i = 0; i < block_indent + 2; i++) indent_string += ' ';

    for (size_t i = 0; i < stmts.size(); i++) {
        if (i > 0) out << '\n';
        out << indent_string << stmts[i].toCode(block_indent);
    }

    return out.str();
}