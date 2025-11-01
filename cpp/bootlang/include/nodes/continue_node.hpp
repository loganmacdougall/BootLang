#pragma once

#include "node.hpp"

class ContinueNode : public Node {
public:
    ContinueNode(size_t lineno, size_t col);

    std::string toCode(int indent) const override;
};
