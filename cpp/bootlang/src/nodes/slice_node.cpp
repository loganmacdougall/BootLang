#include "nodes/slice_node.hpp"

SliceNode::SliceNode(uint32_t lineno, uint32_t col, Node left, SliceRangeNode slice)
: Node(lineno, col, NodeType::n_SLICE), left(left), slice(slice) {}

std::string SliceNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << left.toCode(indent) << slice.toCode(indent);

    return out.str();
}