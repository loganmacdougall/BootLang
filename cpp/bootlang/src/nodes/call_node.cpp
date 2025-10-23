#include "nodes/call_node.hpp"

CallNode::CallNode(uint32_t lineno, uint32_t col, Node left, std::vector<Node> args)
    : Node(lineno, col, NodeType::n_CALL), left(left), args(std::move(args)) {}

std::string CallNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << left.toCode(indent) << '(';
    for (size_t i = 0; i < args.size(); i++) {
        if (i > 0) out << ", ";
        out << args[i].toCode(indent);
    }
    out << ')';

    return out.str();
}