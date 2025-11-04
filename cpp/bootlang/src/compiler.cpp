#include "compiler.hpp"

using INST = Instruction::Type;

Compiler::Compiler() : opRegistry(OpRegistry::GetInstance()) {}

Program Compiler::compile(const BlockNodePtr &ast) {
    top_context = std::make_shared<TopLevelContext>();
    c = top_context;
    
    funcs = std::make_shared<std::vector<std::shared_ptr<CodeObject>>>();
    line_numbers = std::make_shared<std::unordered_map<size_t, size_t>>();
    
    compileTopBlock(ast);

    Program program(top_context, funcs, line_numbers);
    
    return program;
}

void Compiler::compileTopBlock(const BlockNodePtr &ast) {
    compileBlock(ast.get());

    if (c->instructions.size() == 0 || c->instructions.back().type != INST::RETURN) {
        c->emit(INST::PUSH_NULL);
        c->emit(INST::RETURN);
    }
}

void Compiler::compileNode(const Node* node) {
    switch (node->type) {
        case Node::Type::BLOCK:
            compileBlock(Node::toDerived<BlockNode>(node));
            break;
        case Node::Type::VAR:
            compileVar(Node::toDerived<VarNode>(node));
            break;
        case Node::Type::BOOL:
            compileBool(Node::toDerived<BoolNode>(node));
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
        case Node::Type::INDEX:
            compileIndex(Node::toDerived<IndexNode>(node));
            break;
        case Node::Type::DICT_LITERAL:
            compileDictLiteral(Node::toDerived<DictLiteralNode>(node));
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
        case Node::Type::UNARY_OP:
            compileUnaryOp(Node::toDerived<UnaryOpNode>(node));
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
        case Node::Type::FOR:
            compileFor(Node::toDerived<ForNode>(node));
            break;
        case Node::Type::FUNCTION_DEFINITION:
            compileFunction(Node::toDerived<FunctionDefinitionNode>(node));
            break;
        case Node::Type::RETURN:
            compileReturn(Node::toDerived<ReturnNode>(node));
            break;
        case Node::Type::CONTINUE:
            compileContinue(Node::toDerived<ContinueNode>(node));
            break;
        case Node::Type::BREAK:
            compileBreak(Node::toDerived<BreakNode>(node));
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
        line_numbers->insert({stmt->lineno, c->len()});
        compileToplevelNode(stmt.get());
    }
}

void Compiler::compileVar(const VarNode* node) {
    c->loadIdentifier(node->name);
}

void Compiler::compileBool(const BoolNode* node) {
    BoolValue value = BoolValue(node->value);
    size_t id = c->idConstant(&value);
    c->emit(INST::LOAD_CONST, id);
}

void Compiler::compileInt(const IntNode* node) {
    if (node->value < 0) {
        c->emit(INST::LOAD_INT, -node->value);
        c->emit(INST::UNARY_OP, Token::Type::MINUS);
    } else {
        c->emit(INST::LOAD_INT, node->value);
    }
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
    
    size_t id = c->idName(node->property);
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

void Compiler::compileIndex(const IndexNode* node) {
    compileNode(node->left.get());
    compileNode(node->right.get());
    c->emit(INST::LOAD_INDEX);
}

size_t Compiler::pushNodes(const std::vector<NodePtr>& elems) {
    for (auto &elem : elems) {
        compileNode(elem.get());
    }

    return elems.size();
}

void Compiler::compileDictLiteral(const DictLiteralNode* node) {
    for (auto &elem : node->elems) {
        compileNode(elem.first.get());
        compileNode(elem.second.get());
    }
    
    c->emit(INST::BUILD_MAP, node->elems.size());
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
    if (node->constant) {
        Value::Ptr value = createConstant(node);
        if (value != nullptr) {
            compileConstantAtom(value);
            return;
        }
    }

    compileNode(node->left.get());
    compileNode(node->right.get());
    
    c->emit(INST::BINARY_OP, node->op);
}

void Compiler::compileUnaryOp(const UnaryOpNode* node) {
    if (node->constant) {
        Value::Ptr value = createConstant(node);
        if (value != nullptr) {
            compileConstantAtom(value);
            return;
        }
    }
    
    compileNode(node->right.get());
    c->emit(INST::UNARY_OP, node->op);
}

Value::Ptr Compiler::createConstant(const Node* node) {
    switch (node->type) {
        case Node::Type::BINARY_OP:
            return createConstantBinaryOp(Node::toDerived<BinaryOpNode>(node));
        case Node::Type::UNARY_OP:
            return createConstantUnaryOp(Node::toDerived<UnaryOpNode>(node));
        case Node::Type::INT:
            return std::make_unique<IntValue>(Node::toDerived<IntNode>(node)->value);
        case Node::Type::FLOAT:
            return std::make_unique<FloatValue>(Node::toDerived<FloatNode>(node)->value);
        case Node::Type::BOOL:
            return std::make_unique<BoolValue>(Node::toDerived<BoolNode>(node)->value);
        case Node::Type::STRING:
            return std::make_unique<StringValue>(Node::toDerived<StringNode>(node)->value);
        default:
            return nullptr;
    }
}

Value::Ptr Compiler::createConstantBinaryOp(const BinaryOpNode* node) {
    Value::Ptr left_const = createConstant(node->left.get());
    if (left_const == nullptr) return nullptr;
    
    Value::Ptr right_const = createConstant(node->right.get());
    if (right_const == nullptr) return nullptr;

    auto func = opRegistry.get(left_const->type, right_const->type, node->op);
    if (!func.has_value()) {
        opRegistry.throwNotFunction(left_const->type, right_const->type, node->op);
    }
    
    return func.value()(left_const, right_const);
}

Value::Ptr Compiler::createConstantUnaryOp(const UnaryOpNode* node) {
    Value::Ptr right_const = createConstant(node->right.get());
    if (right_const == nullptr) return nullptr;

    auto func = opRegistry.get(right_const->type, node->op);
    if (!func.has_value()) {
        opRegistry.throwNotFunction(right_const->type, node->op);
    }
    
    return func.value()(right_const);
}

void Compiler::compileConstantAtom(Value::Ptr base_value) {
    switch (base_value->type) {
        case Value::Type::INT: {
            long value = Value::toDerived<IntValue>(base_value)->value;
            if (value < 0) {
                c->emit(INST::LOAD_INT, -value);
                c->emit(INST::UNARY_OP, Token::Type::MINUS);
            } else {
                c->emit(INST::LOAD_INT, value);
            }
            break;
        }
        case Value::Type::FLOAT:
        case Value::Type::BOOL:
        case Value::Type::STRING: {
            size_t id = c->idConstant(base_value.get());
            c->emit(INST::LOAD_CONST, id);
            break;
        }
        default:
            throw std::runtime_error("Attempted to compile non-constant atom as constant atom");
    }
}

void Compiler::compileAssign(const AssignNode* node) {
    Token::Type op = node->op;
    
    if (op == Token::Type::ASSIGN) {
        compileNode(Node::toBase(node->right.get()));
        compileAssignIdent(Node::toBase(node->left.get()), op);
    } else {
        if (node->left->type == Node::Type::TUPLE_LITERAL) {
            throw std::runtime_error("Tuple doesn't support augmented assignment");
        }

        compileNode(Node::toBase(node->left.get()));
        compileNode(Node::toBase(node->right.get()));
        c->emit(INST::BINARY_OP, node->op);
        compileAssignIdent(Node::toBase(node->left.get()), op);
    }
}

void Compiler::compileAssignIdent(const Node* node, Token::Type op) {
    switch (node->type) {
        case Node::Type::VAR: 
        {
            const VarNode* left_node = Node::toDerived<VarNode>(node);
            c->storeIdentifier(left_node->name);
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
            c->emit(INST::STORE_ATTR, false_load.arg);

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
                compileAssignIdent(elems->at(i).get(), op);
            }
            break;
        }

        case Node::Type::SLICE: {
            compileSlice(Node::toDerived<SliceNode>(node));
            c->burn();
            c->emit(INST::STORE_INDEX);
            break;
        }

        default:
            throw std::runtime_error("Assigning to type which assignment is not implemented");

    }
}

void Compiler::compileCall(const CallNode* node) {
    if (node->left->type != Node::Type::PROPERTY_ACCESS) {
        compileNode(node->left.get());
        c->emit(INST::PUSH_NULL);
    } else {
        auto prop_access = Node::toDerived<PropertyAccessNode>(node->left.get());
        compileNode(prop_access->left.get());
        c->emit(INST::COPY, 1);
        
        size_t id = c->idName(prop_access->property);
        c->emit(INST::LOAD_ATTR, id);
        c->emit(INST::SWAP, 1);
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

    c->patch(jump_to_else, INST::JUMP_IF_FALSE, jump_to_end - jump_to_else);
    c->patch(jump_to_end, INST::JUMP, end - jump_to_end - 1);
}

void Compiler::compileIf(const IfNode* node) {
    std::vector<size_t> jump_to_end;
    std::vector<size_t> jump_next_ins;
    std::vector<size_t> jump_next_pos;
    
    compileNode(node->if_block.first.get());
    jump_next_ins.push_back(c->emit(INST::JUMP_IF_FALSE));
    compileBlock(node->if_block.second.get());
    jump_to_end.push_back(c->emit(INST::JUMP));
    jump_next_pos.push_back(c->len());

    for (auto& [elif_cond, elif_body] : node->elif_blocks) {
        compileNode(elif_cond.get());
        jump_next_ins.push_back(c->emit(INST::JUMP_IF_FALSE));
        compileBlock(elif_body.get());
        jump_to_end.push_back(c->emit(INST::JUMP));
        jump_next_pos.push_back(c->len());
    }

    if (node->else_block) {
        compileBlock(node->else_block.value().get());
    }

    size_t end = c->len();

    for (size_t pos : jump_to_end) {
        c->patch(pos, INST::JUMP, end - pos - 1);
    }

    for (size_t i = 0; i < jump_next_ins.size(); i++) {
        size_t ins_pos = jump_next_ins[i];
        size_t to_pos = jump_next_pos[i];
        c->patch(ins_pos, INST::JUMP_IF_FALSE, to_pos - ins_pos - 1);
    }
}

void Compiler::compileWhile(const WhileNode* node) {
    size_t loop_start = c->len();

    loop_stack.emplace_back();
    
    compileNode(node->cond.get());
    size_t jump_to_end = c->emit(INST::JUMP_IF_FALSE);
    compileBlock(node->block.get());
    
    size_t last_inst = c->len();
    c->emit(INST::JUMP_BACKWARDS, last_inst - loop_start + 1);
    c->patch(jump_to_end, INST::JUMP_IF_FALSE, last_inst - jump_to_end);
    patchLoopControls(loop_start, last_inst + 1);
}

void Compiler::compileFor(const ForNode* node) {
    size_t start_for, end_for, jump_back;
    
    compileNode(node->iterable.get());
    c->emit(INST::TO_ITER);
    start_for = c->emit(INST::FOR_ITER);
    compileAssignIdent(node->target.get());
    
    loop_stack.emplace_back();
    
    compileBlock(node->block.get());

    jump_back = c->len();
    c->emit(INST::JUMP_BACKWARDS, jump_back - start_for + 1);
    end_for = c->len();

    c->patch(start_for, INST::FOR_ITER, end_for - start_for - 1);
    patchLoopControls(start_for, end_for);
}

void Compiler::compileFunction(const FunctionDefinitionNode* node) {
    std::shared_ptr<FunctionContext> parent = (c == top_context) ? nullptr : std::static_pointer_cast<FunctionContext>(c);
    auto func_context = std::make_shared<FunctionContext>(FunctionContext(top_context, parent));

    auto args_copy = node->args;

    auto code = std::make_shared<CodeObject>(CodeObject(
        node->name,
        node->doc,
        std::move(args_copy),
        func_context
    ));
    funcs->push_back(code);

    for (std::string arg : node->args) {
        func_context->idName(arg);
    }
    
    std::shared_ptr<Context> parent_context = c;
    c = func_context;

    compileTopBlock(node->block);

    c = parent_context;
    
    FunctionValue value = FunctionValue(code);
    size_t id = c->idConstant(&value);

    if (!func_context->freevars.empty()) {
        for (auto freevar : func_context->freevars) {
            c->loadIdentifier(freevar);
        }
        c->emit(INST::BUILD_TUPLE, func_context->freevars.size());
        c->emit(INST::LOAD_CONST, id);
        c->emit(INST::MAKE_CLOSURE);
    } else {
        c->emit(INST::LOAD_CONST, id);
    }

    c->storeIdentifier(node->name);
}

void Compiler::compileBreak(const BreakNode* node) {
    (void)node;
    if (loop_stack.empty()) {
        throw std::runtime_error("Can only break out of a loop");
    }

    size_t pos = c->emit(INST::JUMP);
    loop_stack.back().breaks.push_back(pos);
}

void Compiler::compileContinue(const ContinueNode* node) {
    (void)node;
    if (loop_stack.empty()) {
        throw std::runtime_error("Can only break out of a loop");
    }

    size_t pos = c->emit(INST::JUMP_BACKWARDS);
    loop_stack.back().continues.push_back(pos);
}

void Compiler::compileReturn(const ReturnNode* node) {
    if (node->value.has_value()) {
        compileNode(node->value.value().get());
    } else {
        c->emit(INST::PUSH_NULL);
    }

    c->emit(INST::RETURN);
}

void Compiler::patchLoopControls(size_t loop_start, size_t loop_end) {
    LoopFrame frame = loop_stack.back();
    loop_stack.pop_back();

    for (size_t pos : frame.breaks) {
        c->patch(pos, INST::JUMP, loop_end - pos - 1);
    }

    for (size_t pos : frame.continues) {
        c->patch(pos, INST::JUMP_BACKWARDS, pos - loop_start + 1);
    }
}