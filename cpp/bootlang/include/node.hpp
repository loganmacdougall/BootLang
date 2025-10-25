#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <memory>

enum NodeType {
    n_INT,
    n_FLOAT,
    n_STRING,
    n_BOOL,
    n_VAR,
    n_SLICE_RANGE,
    n_BREAK,
    n_CONTINUE,
    n_RETURN,
    n_BINARY_OP,
    n_UNARY_OP,
    n_ASSIGN,
    n_TERNARY,
    n_BLOCK,
    n_CALL,
    n_SLICE,
    n_PROPERTY_ACCESS,
    n_INDEX,
    n_FUNCTION_DEFINITION,
    n_LIST_LITERAL,
    n_DICT_LITERAL,
    n_SET_LITERAL,
    n_TUPLE_LITERAL,
    n_IF,
    n_WHILE,
    n_FOR
};

typedef std::unique_ptr<class Node> NodePtr;

class Node {
public:
    uint32_t lineno;
    uint32_t col;
    NodeType nodetype;

    Node(uint32_t lineno, uint32_t col, NodeType nodetype);
    virtual ~Node() = default;

    virtual std::string toCode(int indent) const;
};