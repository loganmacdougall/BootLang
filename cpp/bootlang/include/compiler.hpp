#pragma once

#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include "values/all_values.hpp"
#include "nodes/all_nodes.hpp"
#include "token.hpp"
#include "environment.hpp"
#include "instruction.hpp"
#include "program.hpp"



class Compiler {
    TopLevelContext* c = nullptr;
    Environment& env;

public:
    Compiler(Environment& env);
    Program compile(const BlockNodePtr &ast);

private:    
    void compileNode(const Node* node);
    void compileToplevelNode(const Node* node);
    void compileBlock(const BlockNode* node);
    void compileVar(const VarNode* node);
    void compileInt(const IntNode* node);
    void compileFloat(const FloatNode* node);
    void compileString(const StringNode* node);
    void compilePropertyAccess(const PropertyAccessNode* node);
    void compileSlice(const SliceNode* node);
    
    size_t pushNodes(const std::vector<NodePtr>& elems);
    size_t pushNodes(const std::map<NodePtr, NodePtr>& elems);
    size_t pushNodes(const std::set<NodePtr>& elems);
    void compileIndex(const IndexNode* node);
    void compileTupleLiteral(const TupleLiteralNode* node);
    void compileListLiteral(const ListLiteralNode* node);
    void compileSetLiteral(const ListLiteralNode* node);
    void compileMapLiteral(const ListLiteralNode* node);
    
    void compileBinaryOp(const BinaryOpNode* node);
    
    void compileAssign(const AssignNode* node);
    void compileAssignLeft(const Node* node, Token::Type op, const Node* right);

    void compileCall(const CallNode* node);
    
    void compileTernary(const TernaryNode* node);
    void compileIf(const IfNode* node);
    void compileWhile(const WhileNode* node);
    void compileFunction(const FunctionDefinitionNode* node);
    void compileReturn(const ReturnNode* node);
};