#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <sstream>
#include <memory>

typedef std::unique_ptr<class Node> NodePtr;

class Node {
public:
enum Type {
    BLOCK,
    BOOL,
    INT,
    FLOAT,
    STRING,
    ASSIGN,
    TUPLE_LITERAL,
    LIST_LITERAL,
    DICT_LITERAL,
    SET_LITERAL,
    TERNARY,
    BINARY_OP,
    UNARY_OP,
    CALL,
    BREAK,
    CONTINUE,
    RETURN,
    YIELD,
    VAR,
    FUNCTION_DEFINITION,
    IF,
    WHILE,
    FOR,
    INDEX,
    SLICE,
    PROPERTY_ACCESS,
    LITERAL
};

static const size_t NODE_COUNT = static_cast<size_t>(Type::LITERAL) + 1;

public:
    size_t lineno;
    size_t col;
    Node::Type type;
    bool constant;

    Node(size_t lineno, size_t col, Node::Type type, bool constant = false);
    const std::string& typeString() const;
    virtual ~Node() = default;

    template<typename T>
    static std::shared_ptr<T> toDerived(std::shared_ptr<Node> node) {
        return static_pointer_cast<std::shared_ptr<T>>(node);
    }

    template<typename T>
    static inline std::shared_ptr<Node> toBase(std::shared_ptr<T> node) {
        return static_pointer_cast<std::shared_ptr<Node>>(node);
    }

    template<typename T>
    static const T* toDerived(const Node* node) {
        return static_cast<const T*>(node);
    }

    template<typename T>
    static inline const Node* toBase(const T* node) {
        return static_cast<const Node*>(node);
    }

    virtual std::string toCode(int indent) const;
};

class NodeMetadata {
  public:
    typedef std::pair<Node::Type, std::string> RegexPair;
    
  public:
    static const NodeMetadata& GetInstance();
    const std::string& GetNodeName(Node::Type token) const;
    
  private:
    NodeMetadata() = default;

    const std::array<std::string, Node::NODE_COUNT> node_names{
        "BLOCK", "BOOL", "INT", "FLOAT", "STRING",
        "ASSIGN", "TUPLE_LITERAL", "LIST_LITERAL", "DICT_LITERAL", "SET_LITERAL",
        "TERNARY", "BINARY_OP", "UNARY_OP", "CALL",
        "BREAK", "CONTINUE", "RETURN", "YIELD",
        "VAR", "FUNCTION_DEFINITION",
        "IF", "WHILE", "FOR", "INDEX", "SLICE", "PROPERTY_ACCESS", "LITERAL"
    };
    
  public:
    NodeMetadata(NodeMetadata const&) = delete;
    void operator=(NodeMetadata const&) = delete;
};