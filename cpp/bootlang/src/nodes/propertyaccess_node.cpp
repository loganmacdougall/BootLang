#include "nodes/propertyaccess_node.hpp"

PropertyAccessNode::PropertyAccessNode(size_t lineno, size_t col, NodePtr&& left, std::string property)
: Node(lineno, col, Node::Type::PROPERTY_ACCESS), left(std::move(left)), property(property) {}

std::string PropertyAccessNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << left->toCode(indent) << '.' << property;

    return out.str();
}