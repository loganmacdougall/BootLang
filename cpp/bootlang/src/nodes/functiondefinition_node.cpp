#include "nodes/functiondefinition_node.hpp"

FunctionDefinitionNode::FunctionDefinitionNode(size_t lineno, size_t col, std::string name, std::vector<std::string>&& args, BlockNodePtr&& block, std::string doc)
: Node(lineno, col, Node::Type::FUNCTION_DEFINITION), name(name), args(std::move(args)), block(std::move(block)), doc(doc) {}

std::string FunctionDefinitionNode::toCode(int indent) const {
    std::ostringstream out;

    out << "def " << name << '(';
    for (size_t i = 0; i < args.size(); i++) {
        if (i > 0) out << ", ";
        out << args[i];
    }
    out << "):\n";

    out << block->toCode(indent + 2);

    return out.str();
}