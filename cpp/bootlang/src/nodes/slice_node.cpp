#include "nodes/slice_node.hpp"

SliceNode::SliceNode(uint32_t lineno, uint32_t col, NodePtr&& left, std::optional<NodePtr>&& start, std::optional<NodePtr>&& end, std::optional<NodePtr>&& step)
: Node(lineno, col, Node::Type::SLICE), left(std::move(left)),
  start(std::move(start)), end(std::move(end)), step(std::move(step)) {}

std::string SliceNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    std::string start_str = start.has_value() ? start.value()->toCode(indent) : "";
    std::string end_str = end.has_value() ? end.value()->toCode(indent) : "";
    std::string step_str = step.has_value() ? step.value()->toCode(indent) : "";

    out << left->toCode(indent) << '[';
    out << start_str << ":" << end_str;

    if (!step_str.empty()) {
        out << ':' << step_str;
    }
    out << "]";

    return out.str();
}