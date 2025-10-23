#include "node.hpp"

Node::Node(uint32_t lineno, uint32_t col, NodeType nodetype) : lineno(lineno), col(col), nodetype(nodetype) {}