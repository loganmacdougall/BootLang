#pragma once

#include <cstdint>
#include <string>
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
    IDENTIFIER,
    LITERAL
};

public:
    uint32_t lineno;
    uint32_t col;
    Node::Type nodetype;

    Node(uint32_t lineno, uint32_t col, Node::Type nodetype);
    virtual ~Node() = default;

    virtual std::string toCode(int indent) const;
};