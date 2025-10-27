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
        case Node::Type::VAR:
            compileVar(Node::toDerived<VarNode>(node));
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
        case Node::Type::SLICE:
            compileSlice(Node::toDerived<SliceNode>(node));
            break;
        case Node::Type::PROPERTY_ACCESS:
            compilePropertyAccess(Node::toDerived<PropertyAccessNode>(node));
            break;
        case Node::Type::TUPLE_LITERAL:
            compileTupleLiteral(Node::toDerived<TupleLiteralNode>(node));
            break;
        case Node::Type::LIST_LITERAL:
            compileListLiteral(Node::toDerived<ListLiteralNode>(node));
            break;
        case Node::Type::BINARY_OP:
            compileBinaryOp(Node::toDerived<BinaryOpNode>(node));
            break;
        case Node::Type::ASSIGN:
            compileAssign(Node::toDerived<AssignNode>(node));
            break;
        case Node::Type::TERNARY:
            compileTernary(Node::toDerived<TernaryNode>(node));
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
        case Node::Type::CALL:
            compileCall(Node::toDerived<CallNode>(node));
            break;
        default:
            std::string msg = "Unknown node type in compileNode: \"" + NodeMetadata::GetInstance().GetNodeName(node->type) + "\"";
            throw std::runtime_error(msg);
    }
}

void Compiler::compileToplevelNode(const Node* node) {
    switch (node->type) {
        case Node::Type::BINARY_OP:
        case Node::Type::CALL:
        case Node::Type::INDEX:
        case Node::Type::SLICE:
        case Node::Type::TERNARY:
        case Node::Type::UNARY_OP:
        case Node::Type::PROPERTY_ACCESS:
        case Node::Type::TUPLE_LITERAL:
        case Node::Type::SET_LITERAL:
        case Node::Type::DICT_LITERAL:
        case Node::Type::LIST_LITERAL:
            compileNode(node);
            c->emit(INST::POP_TOP);
            break;
        case Node::Type::BOOL:
        case Node::Type::VAR:
        case Node::Type::INT:
        case Node::Type::FLOAT:
        case Node::Type::STRING:
            break;
        default:
            compileNode(node);
    }
}

void Compiler::compileBlock(const BlockNode* node) {
    for (auto& stmt : node->stmts) {
        compileToplevelNode(stmt.get());
    }

    if (node->stmts.at(node->stmts.size() - 1)->type != Node::Type::RETURN) {
        c->emit(INST::PUSH_NULL);
        c->emit(INST::RETURN);
    }
}

void Compiler::compileVar(const VarNode* node) {
    size_t id = c->idGlobal(node->name);
    c->emit(INST::LOAD_GLOBAL, id);
}

void Compiler::compileInt(const IntNode* node) {
    c->emit(INST::LOAD_INT, node->value);
}

void Compiler::compileFloat(const FloatNode* node) {
    FloatValue value = FloatValue(node->value);
    size_t id = c->idConstant(&value);
    c->emit(INST::LOAD_CONST, id);
}

void Compiler::compileString(const StringNode* node) {
    StringValue value = StringValue(node->value);
    size_t id = c->idConstant(&value);
    c->emit(INST::LOAD_CONST, id);
}

void Compiler::compilePropertyAccess(const PropertyAccessNode* node) {
    compileNode(node->left.get());
    
    StringValue value = StringValue(node->property);
    size_t id = c->idConstant(&value);
    c->emit(INST::LOAD_ATTR, id);
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
    c->emit(INST::LOAD_INDEX);
}

size_t Compiler::pushNodes(const std::vector<NodePtr>& elems) {
    for (auto &elem : elems) {
        compileNode(elem.get());
    }

    return elems.size();
}

size_t Compiler::pushNodes(const std::map<NodePtr, NodePtr>& elems) {
    for (auto &elem : elems) {
        compileNode(elem.first.get());
        compileNode(elem.second.get());
    }

    return elems.size();
}

size_t Compiler::pushNodes(const std::set<NodePtr>& elems) {
    for (auto &elem : elems) {
        compileNode(elem.get());
    }

    return elems.size();
}

void Compiler::compileIndex(const IndexNode* node) {
    compileNode(node->left.get());
    compileNode(node->right.get());
    c->emit(INST::LOAD_INDEX);

}

void Compiler::compileTupleLiteral(const TupleLiteralNode* node) {
    size_t size = pushNodes(node->elems);
    c->emit(INST::BUILD_TUPLE, size);
}

void Compiler::compileListLiteral(const ListLiteralNode* node) {
    size_t size = pushNodes(node->elems);
    c->emit(INST::BUILD_LIST, size);
}

void Compiler::compileSetLiteral(const ListLiteralNode* node) {
    size_t size = pushNodes(node->elems);
    c->emit(INST::BUILD_SET, size);
}   

void Compiler::compileMapLiteral(const ListLiteralNode* node) {
    size_t size = pushNodes(node->elems);
    c->emit(INST::BUILD_MAP, size);
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
        compileAssignLeft(Node::toBase(node->left.get()), op, node->right.get());
    } else {
        if (node->left->type == Node::Type::TUPLE_LITERAL) {
            throw std::runtime_error("Tuple doesn't support augmented assignment");
        }

        compileNode(Node::toBase(node->left.get()));
        compileNode(Node::toBase(node->right.get()));
        c->emit(INST::BINARY_OP, node->op);
        compileAssignLeft(Node::toBase(node->left.get()), op, node->right.get());
    }
}

void Compiler::compileAssignLeft(const Node* node, Token::Type op, const Node* right) {
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
            compileIndex(Node::toDerived<IndexNode>(node));
            c->burn();
            c->emit(INST::STORE_INDEX);
            break;
        }

        case Node::Type::PROPERTY_ACCESS: {
            compileNode(node);
            Instruction false_load = c->burn();
            c->emit(INST::LOAD_ATTR, false_load.arg);

            break;
        }

        case Node::Type::TUPLE_LITERAL:
        case Node::Type::LIST_LITERAL:
        {
            const std::vector<std::unique_ptr<Node>>* elems;
            if (node->type == Node::Type::TUPLE_LITERAL) {
                const TupleLiteralNode* tuple_node = Node::toDerived<TupleLiteralNode>(node);
                elems = &tuple_node->elems;
            } else {
                const ListLiteralNode* list_node = Node::toDerived<ListLiteralNode>(node);
                elems = &list_node->elems;
            }
            
            
            size_t size = elems->size();
            
            c->emit(INST::UNPACK_SEQUENCE, size);

            for (size_t i = 0; i < elems->size(); i++) {
                compileAssignLeft(elems->at(i).get(), op, right);
            }
            break;
        }

        case Node::Type::SLICE: {
            compileSlice(Node::toDerived<SliceNode>(node));
            c->burn();
            c->emit(INST::STORE_SLICE);
            break;
        }

        default:
            throw std::runtime_error("Assigning to type which assignment is not implemented");

    }
}

void Compiler::compileCall(const CallNode* node) {
    compileNode(node->left.get());

    if (node->left->type != Node::Type::PROPERTY_ACCESS) {
        c->emit(INST::PUSH_NULL);
    }
    
    for (auto &arg : node->args) {
        compileNode(arg.get());
    }

    c->emit(INST::CALL, node->args.size());
}

void Compiler::compileTernary(const TernaryNode* node) {
    size_t jump_to_else;
    size_t jump_to_end;
    size_t end;

    compileNode(node->cond.get());
    jump_to_else = c->emit(INST::JUMP_IF_FALSE);
    compileNode(node->left.get());
    jump_to_end = c-> emit(INST::JUMP);
    compileNode(node->right.get());
    end = c->len();

    c->patch(jump_to_else, INST::JUMP_IF_FALSE, jump_to_end - jump_to_else + 1);
    c->patch(jump_to_end, INST::JUMP, end - jump_to_end);
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