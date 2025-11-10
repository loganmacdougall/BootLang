#include "nodes/class_node.hpp"

ClassNode::ClassNode(size_t lineno, size_t col, std::string name, BlockNodePtr&& block, std::string doc)
: Node(lineno, col, Node::Type::CLASS), name(name), block(std::move(block)), doc(doc) {}

std::string ClassNode::toCode(int indent) const {
    std::ostringstream out;

    out << "class " << name << "():\n";

    out << block->toCode(indent + 2);

    return out.str();
}