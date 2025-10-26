#include "compiler.hpp"

#define TO_DERIVED(T, node) static_cast<const T*>(node)

Compiler::Compiler() {}

Program Compiler::compile(const BlockNodePtr &ast) {
    instructions = new std::vector<Instruction>();
    line_starts = new std::vector<size_t>();
    
    compileBlock(ast.get());

    Program program(std::move(*instructions), std::move(*line_starts));

    delete instructions;
    delete line_starts;

    return program;
}

void Compiler::emit(Instruction::Type type, uint8_t arg) {
    instructions->emplace_back(type, arg);
}

void Compiler::compileNode(const Node* node) {
    switch (node->nodetype) {
        case Node::Type::BLOCK:
            compileBlock(TO_DERIVED(BlockNode, node));
            break;
        case Node::Type::INT:
            compileInt(TO_DERIVED(IntNode, node));
            break;
        case Node::Type::FLOAT:
            compileFloat(TO_DERIVED(FloatNode, node));
            break;
        case Node::Type::STRING:
            compileString(TO_DERIVED(StringNode, node));
            break;
        case Node::Type::BINARY_OP:
            compileBinaryOp(TO_DERIVED(BinaryOpNode, node));
            break;
        case Node::Type::ASSIGN:
            compileAssign(TO_DERIVED(AssignNode, node));
            break;
        case Node::Type::IF:
            compileIf(TO_DERIVED(IfNode, node));
            break;
        case Node::Type::WHILE:
            compileWhile(TO_DERIVED(WhileNode, node));
            break;
        case Node::Type::FUNCTION_DEFINITION:
            compileFunction(TO_DERIVED(FunctionDefinitionNode, node));
            break;
        case Node::Type::RETURN:
            compileReturn(TO_DERIVED(ReturnNode, node));
            break;
        default:
            throw std::runtime_error("Unknown node type in compileNode");
    }
}

void Compiler::compileBlock(const BlockNode* node) {
    for (auto& stmt : node->stmts) {
        compileNode(stmt.get());
    }
}

void compileInt(const IntNode* node) {

}

void compileFloat(const FloatNode* node) {

}

void compileString(const StringNode* node) {

}

void compileBinaryOp(const BinaryOpNode* node) {

}

void compileAssign(const AssignNode* node) {

}

void compileIf(const IfNode* node) {

}

void compileWhile(const WhileNode* node) {

}

void compileFunction(const FunctionDefinitionNode* node) {

}

void compileReturn(const ReturnNode* node) {

}