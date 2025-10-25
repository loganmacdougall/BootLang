#include "node.hpp"

Node::Node(uint32_t lineno, uint32_t col, Node::Type nodetype) : lineno(lineno), col(col), nodetype(nodetype) {}

std::string Node::toCode(int indent) const {
    (void)indent;
    return "";
}