#include "node.hpp"

Node::Node(uint32_t lineno, uint32_t col, Node::Type type) : lineno(lineno), col(col), type(type) {}

std::string Node::toCode(int indent) const {
    (void)indent;
    return "";
}

const NodeMetadata& NodeMetadata::GetInstance() {
    static NodeMetadata instance;
    return instance;
}

const std::string& NodeMetadata::GetNodeName(Node::Type token) const {
    return node_names[static_cast<size_t>(token)];
}
