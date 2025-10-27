#include "node.hpp"

Node::Node(uint32_t lineno, uint32_t col, Node::Type type) : lineno(lineno), col(col), type(type) {}

std::string Node::toCode(int indent) const {
    (void)indent;
    return "";
}