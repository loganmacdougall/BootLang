#include "nodes/call_node.hpp"

CallNode::CallNode(size_t lineno, size_t col, NodePtr&& left, std::vector<NodePtr>&& args)
    : Node(lineno, col, Node::Type::CALL), left(std::move(left)), args(std::move(args)) {}

std::string CallNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << left->toCode(indent) << '(';
    for (size_t i = 0; i < args.size(); i++) {
        if (i > 0) out << ", ";
        out << args[i]->toCode(indent);
    }
    out << ')';

    return out.str();
}