#include "nodes/functiondefinition_node.hpp"

FunctionDefinitionNode::FunctionDefinitionNode(uint32_t lineno, uint32_t col, std::string name, std::vector<std::string> args, BlockNode block)
: Node(lineno, col, NodeType::n_FLOAT), name(name), args(std::move(args)), block(block) {}

std::string FunctionDefinitionNode::toCode(int indent) const {
    std::ostringstream out;

    out << "def " << name << '(';
    for (size_t i = 0; i < args.size(); i++) {
        if (i > 0) out << ", ";
        out << args[i];
    }
    out << "):\n";

    out << block.toCode(indent + 2);

    return out.str();
}