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

TokenData Parser::consume(std::optional<TokenType> expected) {
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

        if (token.token == TokenType::t_NEWLINE) {
            consume(TokenType::t_NEWLINE);
            continue;
        }

        stmts.push_back(parseStatement());
    }
    
    return std::make_unique<BlockNode>(BlockNode(0, 0, std::move(stmts)));
}

BlockNodePtr Parser::parseBlock() {
    consume(optType(TokenType::t_NEWLINE));
    TokenData start_token = consume(optType(TokenType::t_INDENT));

    std::vector<NodePtr> stmts;
    while (more() && look().token != TokenType::t_DEDENT) {
        if (look().token == TokenType::t_NEWLINE) {
            consume(optType(TokenType::t_NEWLINE));
            continue;
        }
        stmts.push_back(parseStatement());
    }

    consume(optType(TokenType::t_DEDENT));
    return std::make_unique<BlockNode>(BlockNode(start_token.lineno, start_token.col, std::move(stmts)));
}

NodePtr Parser::parseStatement() {
    TokenData token = look();
    TokenData start_token;

    switch (token.token) {
        case t_IDENT:
            return parseAssignmentOrExpression();
        case t_BREAK:
            start_token = consume();
            return std::make_unique<BreakNode>(BreakNode(start_token.lineno, start_token.col));
        case t_CONTINUE:
            start_token = consume();
            return std::make_unique<ContinueNode>(ContinueNode(start_token.lineno, start_token.col));
        case t_RETURN:
            return parseReturn();
        case t_IF:
            return parseIf();
        case t_WHILE:
            return parseWhile();
        case t_FOR:
            return parseFor();
        case t_DEF:
            return parseDef();
        default:
            return parseExpression();
    }
}

NodePtr Parser::parseIdentifier() {
    TokenData token = consume(optType(TokenType::t_IDENT));
    NodePtr node = std::make_unique<VarNode>(VarNode(token.lineno, token.col, token.text));

    while (true) {
        token = look();
        if (token.token == TokenType::t_LPAREN) {
            node = parseCall(std::move(node));
        } else if (token.token == TokenType::t_LBRACK) {
            node = parseIndexOrSlice(std::move(node));
        } else if (token.token == TokenType::t_DOT) {
            node = parsePropertyAccess(std::move(node));
        } else {
            break;
        }
    }

    return node;
}

std::unique_ptr<CallNode> Parser::parseCall(NodePtr&& left) {
    consume(optType(TokenType::t_LPAREN));
    std::vector<NodePtr> args = parseExpressionList(TokenType::t_RPAREN);
    consume(optType(TokenType::t_RPAREN));
    return std::make_unique<CallNode>(CallNode(left->lineno, left->col, std::move(left), std::move(args)));
}

NodePtr Parser::parseIndexOrSlice(NodePtr&& left) {
    consume(optType(TokenType::t_LBRACK));

    std::optional<NodePtr> start = std::nullopt;
    std::optional<NodePtr> end = std::nullopt;
    std::optional<NodePtr> step = std::nullopt;

    TokenData token = look();
    if (token.token != TokenType::t_COLON) {
        start = parseExpression();
    }

    if (look().token == TokenType::t_RBRACK) {
        if (start.has_value()) {
            consume(optType(TokenType::t_RBRACK));
            return std::make_unique<IndexNode>(IndexNode(left->lineno, left->col, std::move(left), std::move(start.value())));
        } else {
            throw std::logic_error("Invalid slice syntax: missing start expression");
        }
    }

    consume(optType(TokenType::t_COLON));

    token = look();
    if (token.token != TokenType::t_COLON && token.token != TokenType::t_RBRACK) {
        end = parseExpression();
    }

    token = look();
    if (token.token == TokenType::t_COLON) {
        consume(optType(TokenType::t_COLON));
        step = parseExpression();
    }

    consume(optType(TokenType::t_RBRACK));
    return std::make_unique<SliceNode>(SliceNode(left->lineno, left->col,
        std::move(left), std::move(start), std::move(end), std::move(step)));
}

std::unique_ptr<PropertyAccessNode> Parser::parsePropertyAccess(NodePtr&& left) {
    consume(optType(TokenType::t_DOT));
    std::string right = consume(optType(TokenType::t_IDENT)).text;
    return std::make_unique<PropertyAccessNode>(PropertyAccessNode(
        left->lineno, left->col, std::move(left), std::move(right)));
}

std::vector<NodePtr> Parser::parseExpressionList(TokenType end_token) {
    ignoreWhitespace(true);

    std::vector<NodePtr> args;
    if (look().token != end_token) {
        args.push_back(parseExpression());
        while (look().token == TokenType::t_COMMA) {
            consume(optType(TokenType::t_COMMA));
            if (look().token == end_token) {
                break;
            }
            args.push_back(parseExpression());
        }
    }

    ignoreWhitespace(false);
    return args;
}

std::vector<std::string> Parser::parseIdentifierList() {
    std::vector<std::string> ids;

    if (!more() || look().token != TokenType::t_IDENT) {
        return ids;
    }

    ids.push_back(consume(optType(TokenType::t_IDENT)).text);

    while (more() && look().token == TokenType::t_COMMA) {
        consume(optType(TokenType::t_COMMA));
        ids.push_back(consume(optType(TokenType::t_IDENT)).text);
    }

    return ids;
}

NodePtr Parser::parseAssignmentOrExpression() {
    static std::set<TokenType> ASSIGNMENT_OPERATORS {
        TokenType::t_ASSIGN, TokenType::t_PLUS_ASSIGN,
        TokenType::t_MINUS_ASSIGN, TokenType::t_STAR_ASSIGN,
        TokenType::t_SLASH_ASSIGN, TokenType::t_PERCENT_ASSIGN
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

        if (next_token.value().token == TokenType::t_NEWLINE) {
            break;
        }

        consume();
        next_token = peek();
    }

    pop();

    return is_assignment ? parseAssignment() : parseExpression();
}

std::unique_ptr<AssignNode> Parser::parseAssignment() {
    NodePtr left = parseIdentifier();
    TokenType op = consume().token;
    NodePtr right = parseExpression();

    return std::make_unique<AssignNode>(AssignNode(left->lineno, left->col, std::move(left), std::move(right), op));
}

NodePtr Parser::parseExpression() {
    return parseTernary();
}

NodePtr Parser::parseTernary() {
    NodePtr left = parseBooleanOps();
    while (more() && look().token == TokenType::t_IF) {
        consume(optType(TokenType::t_IF));
        NodePtr cond = parseBooleanOps();
        consume(optType(TokenType::t_ELSE));
        NodePtr right = parseExpression();
        left = std::make_unique<TernaryNode>(TernaryNode(
            left->lineno, left->col, std::move(cond), std::move(left), std::move(right)));
    }
    return left;
}

NodePtr Parser::parseBooleanOps() {
    static std::set<TokenType> BOOLEAN_OPS {
        TokenType::t_AND, TokenType::t_OR
    };
    
    NodePtr left = parseBooleanNot();
    while (more() && BOOLEAN_OPS.find(look().token) != BOOLEAN_OPS.end()) {
        TokenType op = consume().token;
        NodePtr right = parseBooleanNot();
        left = std::make_unique<BinaryOpNode>(BinaryOpNode(
            left->lineno, left->col, std::move(left), std::move(right), op));
    }
    return left;
}

NodePtr Parser::parseBooleanNot() {
    if (more() && look().token == TokenType::t_NOT) {
        TokenData token = consume();
        NodePtr op = parseComparison();
        return std::make_unique<UnaryOpNode>(UnaryOpNode(
            token.lineno, token.col, std::move(op), TokenType::t_NOT));
    }
    return parseComparison();
}

NodePtr Parser::parseComparison() {
    static std::set<TokenType> COMPARISON_OPS {
        TokenType::t_EQUAL, TokenType::t_NEQUAL,
        TokenType::t_LESS, TokenType::t_LESS_EQUAL,
        TokenType::t_GREATER, TokenType::t_GREATER_EQUAL
    };

    NodePtr left = parseAdditive();
    while (more() && COMPARISON_OPS.find(look().token) != COMPARISON_OPS.end()) {
        TokenType op = consume().token;
        NodePtr right = parseAdditive();
        left = std::make_unique<BinaryOpNode>(BinaryOpNode(
            left->lineno, left->col, std::move(left), std::move(right), std::move(op)));
    }
    return left;
}

NodePtr Parser::parseAdditive() {
    static std::set<TokenType> ADDITIVE_OPS {
        TokenType::t_PLUS, TokenType::t_MINUS, TokenType::t_PERCENT
    };

    NodePtr left = parseMultiplicative();
    while (more() && ADDITIVE_OPS.find(look().token) != ADDITIVE_OPS.end()) {
        TokenType op = consume().token;
        NodePtr right = parseMultiplicative();
        left = std::make_unique<BinaryOpNode>(BinaryOpNode(
            left->lineno, left->col, std::move(left), std::move(right), std::move(op)));
    }
    return left;
}

NodePtr Parser::parseMultiplicative() {
    static std::set<TokenType> MULTIPLICATIVE_OPS {
        TokenType::t_STAR, TokenType::t_SLASH
    };

    NodePtr left = parseUnary();
    while (more() && MULTIPLICATIVE_OPS.find(look().token) != MULTIPLICATIVE_OPS.end()) {
        TokenType op = consume().token;
        NodePtr right = parseUnary();
        left = std::make_unique<BinaryOpNode>(BinaryOpNode(
            left->lineno, left->col, std::move(left), std::move(right), std::move(op)));
    }
    return left;
}

NodePtr Parser::parseUnary() {
    if (more()) {
        TokenData token = look();
        TokenType op = token.token;
        if (op == TokenType::t_PLUS || op == TokenType::t_MINUS) {
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
        case TokenType::t_NUMBER:
            consume();
            if (token.text.find('.') != std::string::npos) {
                return std::make_unique<FloatNode>(FloatNode(token.lineno, token.col, std::stof(token.text)));
            }
            return std::make_unique<IntNode>(IntNode(token.lineno, token.col, std::stoi(token.text)));
        case TokenType::t_STRING:
            consume();
            unescaped = unescapeString(token.text.substr(1, token.text.size() - 2));
            return std::make_unique<StringNode>(StringNode(token.lineno, token.col, unescaped));
        case TokenType::t_TRUE:
        case TokenType::t_FALSE:
            consume();
            return std::make_unique<BoolNode>(BoolNode(token.lineno, token.col, token.token == TokenType::t_TRUE));
        case TokenType::t_IDENT:
            return parseIdentifier();
        case TokenType::t_LPAREN:
            consume(optType(TokenType::t_LPAREN));
            node = parseExpression();
            consume(optType(TokenType::t_RPAREN));
            return node;
        case TokenType::t_LBRACK:
            return parseListLiteral();
        case TokenType::t_LBRACE:
            return parseDictOrSetLiteral();
        default:
            std::string token_name = TokenMetadata::GetInstance().GetTokenName(token.token);
            throw std::runtime_error("Unexpected token: " + token_name);
    }
}

std::unique_ptr<ListLiteralNode> Parser::parseListLiteral() {
    TokenData start_token = consume(optType(TokenType::t_LBRACK));

    ignoreWhitespace(true);
    std::vector<NodePtr> args = parseExpressionList(TokenType::t_RBRACK);
    ignoreWhitespace(false);

    consume(optType(TokenType::t_RBRACK));
    return std::make_unique<ListLiteralNode>(ListLiteralNode(
        start_token.lineno, start_token.col, std::move(args)));
}

NodePtr Parser::parseDictOrSetLiteral() {
    TokenData start_node = consume(optType(TokenType::t_LBRACE));
    ignoreWhitespace(true);

    push();

    std::vector<DictPair> pairs;

    if (look().token == TokenType::t_RBRACE) {
        consume(optType(TokenType::t_RBRACE));
        ignoreWhitespace(false);
        pop(false);
        return std::make_unique<DictLiteralNode>(DictLiteralNode(start_node.lineno, start_node.col, std::move(pairs)));
    }

    NodePtr left = parseExpression();
    if (look().token != TokenType::t_COLON) {
        pop();
        std::vector<NodePtr> elems = parseExpressionList(TokenType::t_RBRACE);
        consume(optType(TokenType::t_RBRACE));
        return std::make_unique<SetLiteralNode>(SetLiteralNode(start_node.lineno, start_node.col, std::move(elems)));
    }

    pop(false);

    NodePtr right = parseExpression();
    pairs.push_back(std::make_pair(std::move(left), std::move(right)));

    while (look().token == TokenType::t_COMMA) {
        consume(optType(TokenType::t_COMMA));
        if (look().token == TokenType::t_RBRACE) {
            break;
        }
        left = parseExpression();
        consume(optType(TokenType::t_COLON));
        right = parseExpression();
        pairs.push_back(std::make_pair(std::move(left), std::move(right)));
    }

    consume(optType(TokenType::t_RBRACE));
    ignoreWhitespace(false);

    return std::make_unique<DictLiteralNode>(DictLiteralNode(start_node.lineno, start_node.col, std::move(pairs)));
}

std::unique_ptr<IfNode> Parser::parseIf() {
    TokenData start_token = consume(optType(TokenType::t_IF));
    NodePtr cond = parseExpression();
    consume(optType(TokenType::t_COLON));
    BlockNodePtr block = parseBlock();

    CondBlock if_block = std::make_pair(std::move(cond), std::move(block));
    
    std::vector<CondBlock> elif_blocks;
    while (more() && look().token == TokenType::t_ELIF) {
        consume(optType(TokenType::t_ELIF));
        cond = parseExpression();
        consume(optType(TokenType::t_COLON));
        block = parseBlock();
        elif_blocks.push_back(std::make_pair(std::move(cond), std::move(block)));
    }

    std::optional<BlockNodePtr> else_block = std::nullopt;
    if (more() && look().token == TokenType::t_ELSE) {
        consume(optType(TokenType::t_ELSE));
        consume(optType(TokenType::t_COLON));
        block = parseBlock();
        else_block = std::optional<BlockNodePtr>(std::move(block));
    }

    return std::make_unique<IfNode>(IfNode(
        start_token.lineno, start_token.col,
        std::move(if_block), std::move(elif_blocks), std::move(else_block)));
}

std::unique_ptr<WhileNode> Parser::parseWhile() {
    TokenData start_token = consume(optType(TokenType::t_WHILE));
    NodePtr cond = parseExpression();
    consume(optType(TokenType::t_COLON));
    BlockNodePtr block = parseBlock();

    return std::make_unique<WhileNode>(WhileNode(
        start_token.lineno, start_token.col, std::move(cond), std::move(block)));
}

std::unique_ptr<ForNode> Parser::parseFor() {
    TokenData start_token = consume(optType(TokenType::t_FOR));
    bool parentheses = false;

    if (look().token == TokenType::t_LPAREN) {
        consume(optType(TokenType::t_LPAREN));
        parentheses = true;
    }

    std::vector<std::string> vars = parseIdentifierList();

    if (parentheses) {
        consume(optType(TokenType::t_RPAREN));
    }

    if (vars.empty()) {
        throw std::logic_error("Expected at least one identifier in for loop. Use '_' if you don't want to use the result from the iterable.");
    }

    consume(optType(TokenType::t_IN));
    NodePtr iterable = parseExpression();
    consume(optType(TokenType::t_COLON));
    BlockNodePtr block = parseBlock();

    return std::make_unique<ForNode>(ForNode(
        start_token.lineno, start_token.col,
        std::move(vars), std::move(iterable), std::move(block)));
}

std::unique_ptr<FunctionDefinitionNode> Parser::parseDef() {
    TokenData start_token = consume(optType(TokenType::t_DEF));
    std::string name = consume(optType(TokenType::t_IDENT)).text;
    consume(optType(TokenType::t_LPAREN));
    std::vector<std::string> args = parseIdentifierList();
    consume(optType(TokenType::t_RPAREN));
    consume(optType(TokenType::t_COLON));
    BlockNodePtr block = parseBlock();

    return std::make_unique<FunctionDefinitionNode>(FunctionDefinitionNode(
        start_token.lineno, start_token.col,
        std::move(name), std::move(args), std::move(block)));
}

std::unique_ptr<ReturnNode> Parser::parseReturn() {
    TokenData start_token = consume(optType(TokenType::t_RETURN));

    if (look().token == TokenType::t_NEWLINE) {
        return std::make_unique<ReturnNode>(ReturnNode(start_token.lineno, start_token.col, std::nullopt));
    }

    NodePtr right = parseExpression();

    return std::make_unique<ReturnNode>(ReturnNode(
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