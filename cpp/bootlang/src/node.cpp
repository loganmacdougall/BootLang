#include "node.hpp"

Node::Node(size_t lineno, size_t col, Node::Type type, bool constant)
: lineno(lineno), col(col), type(type), constant(constant) {}

const std::string& Node::typeString() const {
    return NodeMetadata::GetInstance().GetNodeName(type);
}

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
