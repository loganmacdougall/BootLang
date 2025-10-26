#pragma once

#include <vector>
#include <memory>
#include "nodes/all_nodes.hpp"
#include "instruction.hpp"
#include "program.hpp"

class Compiler {
    std::vector<Instruction>* instructions = nullptr;
    std::vector<size_t>* line_starts = nullptr;
public:
    Compiler();
    Program compile(const BlockNodePtr &ast);

private:
    void emit(Instruction::Type type, uint8_t arg = 0);

    void compileNode(const Node* node);
    
    void compileBlock(const BlockNode* node);
    void compileInt(const IntNode* node);
    void compileFloat(const FloatNode* node);
    void compileString(const StringNode* node);
    void compileBinaryOp(const BinaryOpNode* node);
    void compileAssign(const AssignNode* node);
    void compileIf(const IfNode* node);
    void compileWhile(const WhileNode* node);
    void compileFunction(const FunctionDefinitionNode* node);
    void compileReturn(const ReturnNode* node);

};