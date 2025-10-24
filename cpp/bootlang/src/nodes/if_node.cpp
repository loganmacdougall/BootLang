#include "nodes/if_node.hpp"

IfNode::IfNode(uint32_t lineno, uint32_t col, CondBlock&& if_block, std::vector<CondBlock>&& elif_blocks, std::optional<BlockNodePtr>&& else_block)
: Node(lineno, col, NodeType::n_IF), if_block(std::move(if_block)), elif_blocks(std::move(elif_blocks)), else_block(std::move(else_block)) {}

std::string IfNode::toCode(int indent) const {
    std::ostringstream out;

    std::string indent_string = "";
    for (int i = 0; i < indent; i++) indent_string += ' ';

    out << "if " << if_block.first->toCode(indent) << ":\n";
    out << if_block.second->toCode(indent + 2);

    for (size_t i = 0; i < elif_blocks.size(); i++) {
        const CondBlock& cond_block = elif_blocks[i];
        out << '\n' << indent_string << "elif " << cond_block.first->toCode(indent) << ":\n";
        out << cond_block.second->toCode(indent + 2);
    }

    if (else_block.has_value()) {
        const BlockNodePtr& block = else_block.value();
        out << '\n' << indent_string << "else:\n";
        out << block->toCode(indent + 2);
    }

    return out.str();
}