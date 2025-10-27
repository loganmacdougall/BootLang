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
    void compileBlock(const BlockNode* node);
    void compileInt(const IntNode* node);
    void compileFloat(const FloatNode* node);
    void compileString(const StringNode* node);
    void compileSlice(const SliceNode* node);
    
    void compileBinaryOp(const BinaryOpNode* node);
    
    void compileAssign(const AssignNode* node);
    void compileAssignLeft(const Node* node, Token::Type op);
    
    void compileIf(const IfNode* node);
    void compileWhile(const WhileNode* node);
    void compileFunction(const FunctionDefinitionNode* node);
    void compileReturn(const ReturnNode* node);
};