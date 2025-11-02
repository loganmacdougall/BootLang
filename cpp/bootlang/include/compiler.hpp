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
    struct LoopFrame {
        std::vector<size_t> breaks;
        std::vector<size_t> continues;
    };

    std::shared_ptr<Context> c = nullptr;
    std::shared_ptr<TopLevelContext> top_context = nullptr;
    std::shared_ptr<std::vector<std::shared_ptr<CodeObject>>> funcs = nullptr;
    std::shared_ptr<std::unordered_map<size_t, size_t>> line_numbers = nullptr;

    std::vector<LoopFrame> loop_stack;

public:
    Compiler() = default;
    Program compile(const BlockNodePtr &ast);

private:    
    void compileTopBlock(const BlockNodePtr &ast);
    void compileNode(const Node* node);
    void compileToplevelNode(const Node* node);
    void compileBlock(const BlockNode* node);
    void compileVar(const VarNode* node);
    void compileBool(const BoolNode* node);
    void compileInt(const IntNode* node);
    void compileFloat(const FloatNode* node);
    void compileString(const StringNode* node);
    void compilePropertyAccess(const PropertyAccessNode* node);
    void compileSlice(const SliceNode* node);
    void compileIndex(const IndexNode* node);
    
    size_t pushNodes(const std::vector<NodePtr>& elems);
    void compileDictLiteral(const DictLiteralNode* node);
    void compileTupleLiteral(const TupleLiteralNode* node);
    void compileListLiteral(const ListLiteralNode* node);
    void compileSetLiteral(const ListLiteralNode* node);
    void compileMapLiteral(const ListLiteralNode* node);
    
    void compileBinaryOp(const BinaryOpNode* node);
    void compileUnaryOp(const UnaryOpNode* node);
    
    void compileAssign(const AssignNode* node);
    void compileAssignIdent(const Node* node, Token::Type op = Token::ASSIGN);

    void compileCall(const CallNode* node);
    
    void compileTernary(const TernaryNode* node);
    void compileIf(const IfNode* node);
    void compileWhile(const WhileNode* node);
    void compileFor(const ForNode* node);
    void compileFunction(const FunctionDefinitionNode* node);
    void compileBreak(const BreakNode* node);
    void compileContinue(const ContinueNode* node);
    void compileReturn(const ReturnNode* node);
    void patchLoopControls(size_t loop_start, size_t loop_end);
};