#include "compiler.hpp"

using INST = Instruction::Type;

Compiler::Compiler(Environment& env) : env(env) {}

Program Compiler::compile(const BlockNodePtr &ast) {
    TopLevelContext temp_context;
    c = &temp_context;
    
    compileBlock(ast.get());

    Program program(
        std::move(temp_context),
        env);

    c = nullptr;
    
    return program;
}

void Compiler::compileNode(const Node* node) {
    switch (node->type) {
        case Node::Type::BLOCK:
            compileBlock(Node::toDerived<BlockNode>(node));
            break;
        case Node::Type::INT:
            compileInt(Node::toDerived<IntNode>(node));
            break;
        case Node::Type::FLOAT:
            compileFloat(Node::toDerived<FloatNode>(node));
            break;
        case Node::Type::STRING:
            compileString(Node::toDerived<StringNode>(node));
            break;
        case Node::Type::BINARY_OP:
            compileBinaryOp(Node::toDerived<BinaryOpNode>(node));
            break;
        case Node::Type::ASSIGN:
            compileAssign(Node::toDerived<AssignNode>(node));
            break;
        case Node::Type::IF:
            compileIf(Node::toDerived<IfNode>(node));
            break;
        case Node::Type::WHILE:
            compileWhile(Node::toDerived<WhileNode>(node));
            break;
        case Node::Type::FUNCTION_DEFINITION:
            compileFunction(Node::toDerived<FunctionDefinitionNode>(node));
            break;
        case Node::Type::RETURN:
            compileReturn(Node::toDerived<ReturnNode>(node));
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

void Compiler::compileInt(const IntNode* node) {
    Value int_val = IntValue(node->value);
    size_t id = c->idConstant(std::move(int_val));

    c->emit(INST::LOAD_CONST, id);
}

void Compiler::compileFloat(const FloatNode* node) {
    Value int_val = FloatValue(node->value);
    size_t id = c->idConstant(std::move(int_val));

    c->emit(INST::LOAD_CONST, id);
}

void Compiler::compileString(const StringNode* node) {
    Value int_val = StringValue(node->value);
    size_t id = c->idConstant(std::move(int_val));

    c->emit(INST::LOAD_CONST, id);
}

void Compiler::compileSlice(const SliceNode* node) {
    compileNode(node->left.get());
    
    if (node->start.has_value()) {
        compileNode(node->start.value().get());
    } else {
        c->emit(INST::PUSH_NULL);
    }

    if (node->end.has_value()) {
        compileNode(node->end.value().get());
    } else {
        c->emit(INST::PUSH_NULL);
    }

    if (node->step.has_value()) {
        compileNode(node->step.value().get());
    } else {
        c->emit(INST::PUSH_NULL);
    }

    c->emit(INST::BUILD_SLICE);
}

void Compiler::compileBinaryOp(const BinaryOpNode* node) {
    compileNode(Node::toBase(node->left.get()));
    compileNode(Node::toBase(node->right.get()));
    
    c->emit(INST::BINARY_OP, node->op);
}

void Compiler::compileAssign(const AssignNode* node) {
    Token::Type op = node->op;
    
    if (op == Token::Type::ASSIGN) {
        compileNode(Node::toBase(node->right.get()));
        compileAssignLeft(Node::toBase(node->left.get()), op);
    } else {
        if (node->left->type == Node::Type::TUPLE_LITERAL) {
            throw std::runtime_error("Tuple doesn't support augmented assignment");
        }

        compileNode(Node::toBase(node->left.get()));
        compileNode(Node::toBase(node->right.get()));
        c->emit(INST::BINARY_OP, node->op);
        compileAssignLeft(Node::toBase(node->left.get()), op);
    }
}

void Compiler::compileAssignLeft(const Node* node, Token::Type op) {
    switch (node->type) {
        case Node::Type::VAR: 
        {
            const VarNode* left_node = Node::toDerived<VarNode>(node);
            size_t id = c->idGlobal(left_node->name);
            c->emit(INST::STORE_GLOBAL, id);
            break;
        }

        case Node::Type::INDEX: 
        {
            const IndexNode* left_node = Node::toDerived<IndexNode>(node);
            compileNode(left_node->left.get());
            compileNode(left_node->right.get());
            c->emit(INST::STORE_INDEX);
            break;
        }

        case Node::Type::TUPLE_LITERAL:
        {
            const TupleLiteralNode* left_node = Node::toDerived<TupleLiteralNode>(node);
            c->emit(INST::UNPACK_SEQUENCE, left_node->elems.size());
            for (size_t i = 0; i < left_node->elems.size(); i++) {
                compileAssignLeft(left_node->elems[i].get(), op);
            }
            break;
        }

        case Node::Type::SLICE: {
            const SliceNode* left_node = Node::toDerived<SliceNode>(node);
            compileNode(left_node->left.get());
    
            if (left_node->start.has_value()) {
                compileNode(left_node->start.value().get());
            } else {
                c->emit(INST::PUSH_NULL);
            }

            if (left_node->end.has_value()) {
                compileNode(left_node->end.value().get());
            } else {
                c->emit(INST::PUSH_NULL);
            }

            if (left_node->step.has_value()) {
                compileNode(left_node->step.value().get());
            } else {
                c->emit(INST::PUSH_NULL);
            }

            c->emit(INST::STORE_SLICE);
            break;
        }

        default:
            throw std::runtime_error("Assigning to type which assignment is not implemented");

    }
}

void Compiler::compileIf(const IfNode* node) {
    std::vector<size_t> jump_to_end;
    std::vector<size_t> jump_to_next;
    
    compileNode(node->if_block.first.get());
    jump_to_next.push_back(c->emit(INST::JUMP_IF_FALSE));
    compileBlock(node->if_block.second.get());
    jump_to_end.push_back(c->emit(INST::JUMP));

    for (auto& [elif_cond, elif_body] : node->elif_blocks) {
        compileNode(elif_cond.get());
        jump_to_next.push_back(c->emit(INST::JUMP_IF_FALSE));
        compileBlock(elif_body.get());
        jump_to_end.push_back(c->emit(INST::JUMP));
    }

    if (node->else_block) {
        jump_to_next.push_back(c->len());
        compileBlock(node->if_block.second.get());
    }

    size_t end = c->len();

    for (size_t pos : jump_to_end) {
        c->patch(pos, INST::JUMP_IF_FALSE, end - pos);
    }

    for (size_t i = 0; i < jump_to_next.size() - 1; i++) {
        size_t pos = jump_to_next[i];
        size_t next_pos = jump_to_next[i+1];
        c->patch(pos, INST::JUMP_IF_FALSE, next_pos - pos);
    }
}

void Compiler::compileWhile(const WhileNode* node) {
    size_t loop_start = c->len();
    
    compileNode(node->cond.get());
    size_t jump_to_end = c->emit(INST::JUMP_IF_FALSE);
    compileBlock(node->block.get());
    
    size_t last_inst = c->len();
    c->emit(INST::JUMP_BACKWARDS, last_inst - loop_start);
    c->patch(jump_to_end, INST::JUMP_IF_FALSE, last_inst - loop_start + 1);
}

void Compiler::compileFunction(const FunctionDefinitionNode* node) {
    (void)node;

}

void Compiler::compileReturn(const ReturnNode* node) {
    if (node->value.has_value()) {
        compileNode(node->value.value().get());
    } else {
        c->emit(INST::PUSH_NULL);
    }

    c->emit(INST::RETURN);
}