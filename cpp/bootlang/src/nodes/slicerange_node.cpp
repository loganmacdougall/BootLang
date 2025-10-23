#include "nodes/slicerange_node.hpp"

SliceRangeNode::SliceRangeNode(uint32_t lineno, uint32_t col, std::optional<Node> start, std::optional<Node> end, std::optional<Node> step)
    : Node(lineno, col, NodeType::n_SLICE_RANGE), start(start), end(end), step(step) {}

std::string SliceRangeNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    std::string start_str = start ? start.value().toCode(indent) : "";
    std::string end_str = end ? end.value().toCode(indent) : "";
    std::string step_str = step ? step.value().toCode(indent) : "";

    out << '[' << start_str << ":" << end_str;

    if (!step_str.empty()) {
        out << ':' << step_str;
    }
    out << "]";

    return out.str();
}