#include "nodes/propertyaccess_node.hpp"

PropertyAccessNode::PropertyAccessNode(uint32_t lineno, uint32_t col, Node left, std::string property)
: Node(lineno, col, NodeType::n_PROPERTY_ACCESS), left(left), property(property) {}

std::string PropertyAccessNode::toCode(int indent) const {
    (void)indent; 
    std::ostringstream out;

    out << left.toCode(indent) << '.' << property;

    return out.str();
}