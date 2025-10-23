#pragma once

#include "node.hpp"
#include "nodes/slicerange_node.hpp"

struct SliceNode : public Node {
    Node left;
    SliceRangeNode slice;

    SliceNode(uint32_t lineno, uint32_t col, Node left, SliceRangeNode slice);

    std::string toCode(int indent) const override;
};
