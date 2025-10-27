#include "parser.hpp"

Parser::Parser(Tokens tokens)
: tokens(std::move(tokens)), pos(0), ignore_whitespace(false) {}

std::optional<TokenData> Parser::peek() {
    if (ignore_whitespace) passAllWhitespace();

    return (pos < tokens.size() ? std::optional<TokenData>(tokens[pos]) : std::nullopt);
}

TokenData Parser::look() {
    std::optional<TokenData> token = peek();

    if (!token) {
        throw std::range_error("Unexpected end of input");
    }

    return token.value();
}

bool Parser::more() {
    if (ignore_whitespace) passAllWhitespace();
    return pos < tokens.size();
}

void Parser::push() {
    poses.push_back(pos);
}

void Parser::pop(bool replace) {
    if (replace) pos = poses.back();
    poses.pop_back();
}

void Parser::ignoreWhitespace(bool ignore) {
    ignore_whitespace = ignore;
}

void Parser::passAllWhitespace() {
    std::optional<TokenData> token = pos < tokens.size() ? std::optional<TokenData>(tokens[pos]) : std::nullopt;
    while (token.has_value() && WHITESPACE.find(token.value().token) != WHITESPACE.end()) {
        pos += 1;
        token = pos < tokens.size() ? std::optional<TokenData>(tokens[pos]) : std::nullopt;
    }
}

TokenData Parser::consume(std::optional<Token::Type> expected) {
    TokenData token = look();
    if (expected && token.token != expected.value()) {
        std::string expected_string = TokenMetadata::GetInstance().GetTokenName(expected.value());
        std::string actual_string = TokenMetadata::GetInstance().GetTokenName(token.token);
        throw std::logic_error("Expected " + expected_string + ", got " + actual_string);
    }
    
    pos += 1;
    return token;
}

BlockNodePtr Parser::parse() {
    std::vector<NodePtr> stmts;
    while (more()) {
        TokenData token = look();

        if (token.token == Token::Type::NEWLINE) {
            consume(Token::Type::NEWLINE);
            continue;
        }

        stmts.push_back(parseStatement());
    }
    
    return std::make_unique<BlockNode>(BlockNode(0, 0, std::move(stmts)));
}

BlockNodePtr Parser::parseBlock() {
    consume(optType(Token::Type::NEWLINE));
    TokenData start_token = consume(optType(Token::Type::INDENT));

    std::vector<NodePtr> stmts;
    while (more() && look().token != Token::Type::DEDENT) {
        if (look().token == Token::Type::NEWLINE) {
            consume(optType(Token::Type::NEWLINE));
            continue;
        }
        stmts.push_back(parseStatement());
    }

    consume(optType(Token::Type::DEDENT));
    return std::make_unique<BlockNode>(BlockNode(start_token.lineno, start_token.col, std::move(stmts)));
}

NodePtr Parser::parseStatement() {
    TokenData token = look();
    TokenData start_token;

    switch (token.token) {
        case Token::Type::IDENT:
        case Token::Type::LPAREN:
            return parseAssignmentOrExpression();
        case Token::Type::BREAK:
            start_token = consume();
            return std::make_unique<BreakNode>(BreakNode(start_token.lineno, start_token.col));
        case Token::Type::CONTINUE:
            start_token = consume();
            return std::make_unique<ContinueNode>(ContinueNode(start_token.lineno, start_token.col));
        case Token::Type::RETURN:
            return parseReturn();
        case Token::Type::YIELD:
            return parseYield();
        case Token::Type::IF:
            return parseIf();
        case Token::Type::WHILE:
            return parseWhile();
        case Token::Type::FOR:
            return parseFor();
        case Token::Type::DEF:
            return parseDef();
        default:
            return parseExpression();
    }
}

NodePtr Parser::parseIdentifier() {
    NodePtr node;
    TokenData token;
    
    if (look().token == Token::Type::LPAREN) {
        token = consume(optType(Token::Type::LPAREN));
        node = parseIdentifierOrTuple();
        token = consume(optType(Token::Type::RPAREN));
    } else {
        token = consume(optType(Token::Type::IDENT));
        node = std::make_unique<VarNode>(VarNode(token.lineno, token.col, token.text));
    }


    while (true) {
        token = look();
        if (token.token == Token::Type::LPAREN) {
            node = parseCall(std::move(node));
        } else if (token.token == Token::Type::LBRACK) {
            node = parseIndexOrSlice(std::move(node));
        } else if (token.token == Token::Type::DOT) {
            node = parsePropertyAccess(std::move(node));
        } else {
            break;
        }
    }

    return node;
}

std::unique_ptr<CallNode> Parser::parseCall(NodePtr&& left) {
    consume(optType(Token::Type::LPAREN));
    std::vector<NodePtr> args = parseExpressionList(Token::Type::RPAREN);
    consume(optType(Token::Type::RPAREN));
    return std::make_unique<CallNode>(CallNode(left->lineno, left->col, std::move(left), std::move(args)));
}

NodePtr Parser::parseIndexOrSlice(NodePtr&& left) {
    consume(optType(Token::Type::LBRACK));

    std::optional<NodePtr> start = std::nullopt;
    std::optional<NodePtr> end = std::nullopt;
    std::optional<NodePtr> step = std::nullopt;

    TokenData token = look();
    if (token.token != Token::Type::COLON) {
        start = parseExpression();
    }

    if (look().token == Token::Type::RBRACK) {
        if (start.has_value()) {
            consume(optType(Token::Type::RBRACK));
            return std::make_unique<IndexNode>(IndexNode(left->lineno, left->col, std::move(left), std::move(start.value())));
        } else {
            throw std::logic_error("Invalid slice syntax: missing start expression");
        }
    }

    consume(optType(Token::Type::COLON));

    token = look();
    if (token.token != Token::Type::COLON && token.token != Token::Type::RBRACK) {
        end = parseExpression();
    }

    token = look();
    if (token.token == Token::Type::COLON) {
        consume(optType(Token::Type::COLON));
        step = parseExpression();
    }

    consume(optType(Token::Type::RBRACK));
    return std::make_unique<SliceNode>(SliceNode(left->lineno, left->col,
        std::move(left), std::move(start), std::move(end), std::move(step)));
}

std::unique_ptr<PropertyAccessNode> Parser::parsePropertyAccess(NodePtr&& left) {
    consume(optType(Token::Type::DOT));
    std::string right = consume(optType(Token::Type::IDENT)).text;
    return std::make_unique<PropertyAccessNode>(PropertyAccessNode(
        left->lineno, left->col, std::move(left), std::move(right)));
}

std::vector<NodePtr> Parser::parseExpressionList(Token::Type end_token) {
    ignoreWhitespace(true);

    std::vector<NodePtr> args;
    if (look().token != end_token) {
        args.push_back(parseTernary());
        while (look().token == Token::Type::COMMA) {
            consume(optType(Token::Type::COMMA));
            if (look().token == end_token) {
                break;
            }
            args.push_back(parseTernary());
        }
    }

    ignoreWhitespace(false);
    return args;
}

std::vector<std::string> Parser::parseIdentifierList() {
    std::vector<std::string> ids;

    if (!more() || look().token != Token::Type::IDENT) {
        return ids;
    }

    ids.push_back(consume(optType(Token::Type::IDENT)).text);

    while (more() && look().token == Token::Type::COMMA) {
        consume(optType(Token::Type::COMMA));
        ids.push_back(consume(optType(Token::Type::IDENT)).text);
    }

    return ids;
}

NodePtr Parser::parseIdentifierOrTuple() {
    NodePtr first = parseIdentifier();
    if (more() && look().token == Token::Type::COMMA) {
        std::vector<NodePtr> elems;
        elems.push_back(std::move(first));
        
        while (more() && look().token == Token::Type::COMMA) {
            consume(optType(Token::Type::COMMA));
            NodePtr elem = parseIdentifier();
            elems.push_back(std::move(elem));
        }

        return std::make_unique<TupleLiteralNode>(TupleLiteralNode(
            elems[0]->lineno, elems[0]->col, std::move(elems)));
    } else {
        return first;
    }
}

NodePtr Parser::parseAssignmentOrExpression() {
    static std::set<Token::Type> ASSIGNMENT_OPERATORS {
        Token::Type::ASSIGN, Token::Type::PLUS_ASSIGN,
        Token::Type::MINUS_ASSIGN, Token::Type::STAR_ASSIGN,
        Token::Type::SLASH_ASSIGN, Token::Type::PERCENT_ASSIGN,
        Token::Type::DOUBLE_SLASH_ASSIGN
    };

    push();
    
    consume();
    std::optional<TokenData> next_token = peek();
    bool is_assignment = false;

    while (next_token.has_value()) {
        if (ASSIGNMENT_OPERATORS.find(next_token.value().token) != ASSIGNMENT_OPERATORS.end()) {
            is_assignment = true;
            break;
        }

        if (next_token.value().token == Token::Type::NEWLINE) {
            break;
        }

        consume();
        next_token = peek();
    }

    pop();

    return is_assignment ? parseAssignment() : parseExpression();
}

std::unique_ptr<AssignNode> Parser::parseAssignment() {
    NodePtr left = parseIdentifierOrTuple();
    Token::Type op = consume().token;
    NodePtr right = parseExpression();

    return std::make_unique<AssignNode>(AssignNode(left->lineno, left->col, std::move(left), std::move(right), op));
}

NodePtr Parser::parseExpression() {
    return parseTupleExpression();
}

NodePtr Parser::parseTupleExpression() {
    NodePtr first = parseTernary();
    if (more() && look().token == Token::Type::COMMA) {
        std::vector<NodePtr> elems;
        elems.push_back(std::move(first));
        
        while (more() && look().token == Token::Type::COMMA) {
            consume(optType(Token::Type::COMMA));
            NodePtr elem = parseTernary();
            elems.push_back(std::move(elem));
        }

        return std::make_unique<TupleLiteralNode>(TupleLiteralNode(
            elems[0]->lineno, elems[0]->col, std::move(elems)));
    }
    return first;
}

NodePtr Parser::parseTernary() {
    NodePtr left = parseBooleanOps();
    while (more() && look().token == Token::Type::IF) {
        consume(optType(Token::Type::IF));
        NodePtr cond = parseBooleanOps();
        consume(optType(Token::Type::ELSE));
        NodePtr right = parseExpression();
        left = std::make_unique<TernaryNode>(TernaryNode(
            left->lineno, left->col, std::move(left), std::move(right), std::move(cond)));
    }
    return left;
}

NodePtr Parser::parseBooleanOps() {
    static std::set<Token::Type> BOOLEAN_OPS {
        Token::Type::AND, Token::Type::OR
    };
    
    NodePtr left = parseBooleanNot();
    while (more() && BOOLEAN_OPS.find(look().token) != BOOLEAN_OPS.end()) {
        Token::Type op = consume().token;
        NodePtr right = parseBooleanNot();
        left = std::make_unique<BinaryOpNode>(BinaryOpNode(
            left->lineno, left->col, std::move(left), std::move(right), op));
    }
    return left;
}

NodePtr Parser::parseBooleanNot() {
    if (more() && look().token == Token::Type::NOT) {
        TokenData token = consume();
        NodePtr op = parseComparison();
        return std::make_unique<UnaryOpNode>(UnaryOpNode(
            token.lineno, token.col, std::move(op), Token::Type::NOT));
    }
    return parseComparison();
}

NodePtr Parser::parseComparison() {
    static std::set<Token::Type> COMPARISON_OPS {
        Token::Type::EQUAL, Token::Type::NEQUAL,
        Token::Type::LESS, Token::Type::LESS_EQUAL,
        Token::Type::GREATER, Token::Type::GREATER_EQUAL
    };

    NodePtr left = parseAdditive();
    while (more() && COMPARISON_OPS.find(look().token) != COMPARISON_OPS.end()) {
        Token::Type op = consume().token;
        NodePtr right = parseAdditive();
        left = std::make_unique<BinaryOpNode>(BinaryOpNode(
            left->lineno, left->col, std::move(left), std::move(right), std::move(op)));
    }
    return left;
}

NodePtr Parser::parseAdditive() {
    static std::set<Token::Type> ADDITIVE_OPS {
        Token::Type::PLUS, Token::Type::MINUS
    };

    NodePtr left = parseMultiplicative();
    while (more() && ADDITIVE_OPS.find(look().token) != ADDITIVE_OPS.end()) {
        Token::Type op = consume().token;
        NodePtr right = parseMultiplicative();
        left = std::make_unique<BinaryOpNode>(BinaryOpNode(
            left->lineno, left->col, std::move(left), std::move(right), std::move(op)));
    }
    return left;
}

NodePtr Parser::parseMultiplicative() {
    static std::set<Token::Type> MULTIPLICATIVE_OPS {
        Token::Type::STAR, Token::Type::SLASH, Token::Type::DOUBLE_SLASH, Token::Type::PERCENT
    };

    NodePtr left = parseUnary();
    while (more() && MULTIPLICATIVE_OPS.find(look().token) != MULTIPLICATIVE_OPS.end()) {
        Token::Type op = consume().token;
        NodePtr right = parseUnary();
        left = std::make_unique<BinaryOpNode>(BinaryOpNode(
            left->lineno, left->col, std::move(left), std::move(right), std::move(op)));
    }
    return left;
}

NodePtr Parser::parseUnary() {
    if (more()) {
        TokenData token = look();
        Token::Type op = token.token;
        if (op == Token::Type::PLUS || op == Token::Type::MINUS) {
            consume();
            NodePtr right = parseAtom();
            return std::make_unique<UnaryOpNode>(UnaryOpNode(
                token.lineno, token.col, std::move(right), std::move(op)));
        }
    }
    return parseAtom();
}

NodePtr Parser::parseAtom() {
    TokenData token = look();
    std::string unescaped;
    NodePtr node = nullptr;

    switch (token.token) {
        case Token::Type::NUMBER:
            consume();
            if (token.text.find('.') != std::string::npos) {
                return std::make_unique<FloatNode>(FloatNode(token.lineno, token.col, std::stof(token.text)));
            }
            return std::make_unique<IntNode>(IntNode(token.lineno, token.col, std::stoi(token.text)));
        case Token::Type::STRING:
            consume();
            unescaped = unescapeString(token.text.substr(1, token.text.size() - 2));
            return std::make_unique<StringNode>(StringNode(token.lineno, token.col, unescaped));
        case Token::Type::TRUE:
        case Token::Type::FALSE:
            consume();
            return std::make_unique<BoolNode>(BoolNode(token.lineno, token.col, token.token == Token::Type::TRUE));
        case Token::Type::IDENT:
            return parseIdentifier();
        case Token::Type::LPAREN:
            consume(optType(Token::Type::LPAREN));
            node = parseExpression();
            consume(optType(Token::Type::RPAREN));
            return node;
        case Token::Type::LBRACK:
            return parseListLiteral();
        case Token::Type::LBRACE:
            return parseDictOrSetLiteral();
        default:
            std::string token_name = TokenMetadata::GetInstance().GetTokenName(token.token);
            throw std::runtime_error("Unexpected token: " + token_name);
    }
}

std::unique_ptr<ListLiteralNode> Parser::parseListLiteral() {
    TokenData start_token = consume(optType(Token::Type::LBRACK));

    ignoreWhitespace(true);
    std::vector<NodePtr> args = parseExpressionList(Token::Type::RBRACK);
    ignoreWhitespace(false);

    consume(optType(Token::Type::RBRACK));
    return std::make_unique<ListLiteralNode>(ListLiteralNode(
        start_token.lineno, start_token.col, std::move(args)));
}

NodePtr Parser::parseDictOrSetLiteral() {
    TokenData start_node = consume(optType(Token::Type::LBRACE));
    ignoreWhitespace(true);

    push();

    std::vector<DictPair> pairs;

    if (look().token == Token::Type::RBRACE) {
        consume(optType(Token::Type::RBRACE));
        ignoreWhitespace(false);
        pop(false);
        return std::make_unique<DictLiteralNode>(DictLiteralNode(start_node.lineno, start_node.col, std::move(pairs)));
    }

    NodePtr left = parseExpression();
    if (look().token != Token::Type::COLON) {
        pop();
        std::vector<NodePtr> elems = parseExpressionList(Token::Type::RBRACE);
        consume(optType(Token::Type::RBRACE));
        return std::make_unique<SetLiteralNode>(SetLiteralNode(start_node.lineno, start_node.col, std::move(elems)));
    }

    pop(false);

    NodePtr right = parseExpression();
    pairs.push_back(std::make_pair(std::move(left), std::move(right)));

    while (look().token == Token::Type::COMMA) {
        consume(optType(Token::Type::COMMA));
        if (look().token == Token::Type::RBRACE) {
            break;
        }
        left = parseExpression();
        consume(optType(Token::Type::COLON));
        right = parseExpression();
        pairs.push_back(std::make_pair(std::move(left), std::move(right)));
    }

    consume(optType(Token::Type::RBRACE));
    ignoreWhitespace(false);

    return std::make_unique<DictLiteralNode>(DictLiteralNode(start_node.lineno, start_node.col, std::move(pairs)));
}

std::unique_ptr<IfNode> Parser::parseIf() {
    TokenData start_token = consume(optType(Token::Type::IF));
    NodePtr cond = parseExpression();
    consume(optType(Token::Type::COLON));
    BlockNodePtr block = parseBlock();

    CondBlock if_block = std::make_pair(std::move(cond), std::move(block));
    
    std::vector<CondBlock> elif_blocks;
    while (more() && look().token == Token::Type::ELIF) {
        consume(optType(Token::Type::ELIF));
        cond = parseExpression();
        consume(optType(Token::Type::COLON));
        block = parseBlock();
        elif_blocks.push_back(std::make_pair(std::move(cond), std::move(block)));
    }

    std::optional<BlockNodePtr> else_block = std::nullopt;
    if (more() && look().token == Token::Type::ELSE) {
        consume(optType(Token::Type::ELSE));
        consume(optType(Token::Type::COLON));
        block = parseBlock();
        else_block = std::optional<BlockNodePtr>(std::move(block));
    }

    return std::make_unique<IfNode>(IfNode(
        start_token.lineno, start_token.col,
        std::move(if_block), std::move(elif_blocks), std::move(else_block)));
}

std::unique_ptr<WhileNode> Parser::parseWhile() {
    TokenData start_token = consume(optType(Token::Type::WHILE));
    NodePtr cond = parseExpression();
    consume(optType(Token::Type::COLON));
    BlockNodePtr block = parseBlock();

    return std::make_unique<WhileNode>(WhileNode(
        start_token.lineno, start_token.col, std::move(cond), std::move(block)));
}

std::unique_ptr<ForNode> Parser::parseFor() {
    TokenData start_token = consume(optType(Token::Type::FOR));
    bool parentheses = false;

    if (look().token == Token::Type::LPAREN) {
        consume(optType(Token::Type::LPAREN));
        parentheses = true;
    }

    std::vector<std::string> vars = parseIdentifierList();

    if (parentheses) {
        consume(optType(Token::Type::RPAREN));
    }

    if (vars.empty()) {
        throw std::logic_error("Expected at least one identifier in for loop. Use '_' if you don't want to use the result from the iterable.");
    }

    consume(optType(Token::Type::IN));
    NodePtr iterable = parseExpression();
    consume(optType(Token::Type::COLON));
    BlockNodePtr block = parseBlock();

    return std::make_unique<ForNode>(ForNode(
        start_token.lineno, start_token.col,
        std::move(vars), std::move(iterable), std::move(block)));
}

std::unique_ptr<FunctionDefinitionNode> Parser::parseDef() {
    TokenData start_token = consume(optType(Token::Type::DEF));
    std::string name = consume(optType(Token::Type::IDENT)).text;
    consume(optType(Token::Type::LPAREN));
    std::vector<std::string> args = parseIdentifierList();
    consume(optType(Token::Type::RPAREN));
    consume(optType(Token::Type::COLON));
    BlockNodePtr block = parseBlock();

    return std::make_unique<FunctionDefinitionNode>(FunctionDefinitionNode(
        start_token.lineno, start_token.col,
        std::move(name), std::move(args), std::move(block)));
}

std::unique_ptr<ReturnNode> Parser::parseReturn() {
    TokenData start_token = consume(optType(Token::Type::RETURN));

    if (look().token == Token::Type::NEWLINE) {
        return std::make_unique<ReturnNode>(ReturnNode(start_token.lineno, start_token.col, std::nullopt));
    }

    NodePtr right = parseExpression();

    return std::make_unique<ReturnNode>(ReturnNode(
        start_token.lineno, start_token.col,
        std::optional<NodePtr>(std::move(right))));
}

std::unique_ptr<YieldNode> Parser::parseYield() {
    TokenData start_token = consume(optType(Token::Type::YIELD));

    if (look().token == Token::Type::NEWLINE) {
        return std::make_unique<YieldNode>(YieldNode(start_token.lineno, start_token.col, std::nullopt));
    }

    NodePtr right = parseExpression();

    return std::make_unique<YieldNode>(YieldNode(
        start_token.lineno, start_token.col,
        std::optional<NodePtr>(std::move(right))));
}

std::string Parser::unescapeString(std::string str) {
    static std::map<char, std::string> UNESCAPE_MAP {
        {'\'', "'"}, {'\"', "\""}, {'\\', "\\"}, {'n', "\n"},
        {'r', "\r"}, {'t', "\t"},  {'b', "\b"},  {'f', "\f"},
    };

    std::string res = "";
    bool unescape = false;
    
    for (size_t i = 0; i < str.size(); i++) {
        char c = str[i];

        if (c != '\\' && !unescape) {
            res += c;
            continue;
        }
        
        if (c == '\\' && !unescape) {
            unescape = true;
            continue;
        }
        
        auto escaped_iter = UNESCAPE_MAP.find(c);

        if (escaped_iter != UNESCAPE_MAP.end()) {
            res += escaped_iter->second;
        } else if (c == 'x') {
            if (i + 2 >= str.size()) {
                throw std::logic_error("Invalid escape sequence: \\x with insufficient hex digits");
            }
            std::string hex_str = str.substr(i+1, 2);
            char hex_char = static_cast<char>(std::stoi(hex_str, nullptr, 16));
            res += hex_char;
            i += 2;
        } else if (c >= '0' && c <= '7') {
            if (i + 3 >= str.size()) {
                throw std::logic_error("Invalid escape sequence: \\x with insufficient hex digits");
            }
            std::string oct_str = str.substr(i, 3);
            char oct_char = static_cast<char>(std::stoi(oct_str, nullptr, 8));
            res += oct_char;
            i += 3;
        } else {
            res += c;
        }

        unescape = false;
    }

    return res;
}