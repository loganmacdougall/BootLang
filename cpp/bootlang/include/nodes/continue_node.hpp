#pragma once

#include "node.hpp"

class ContinueNode : public Node {
public:
    ContinueNode(uint32_t lineno, uint32_t col);

    std::string toCode(int indent) const override;
};
