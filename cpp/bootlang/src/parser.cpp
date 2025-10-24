#include <iostream>
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
    while (token && WHITESPACE.find(token.value().token) != WHITESPACE.end()) {
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

BlockNode Parser::parse() {
    std::vector<Node> stmts;
    while (more()) {
        TokenData token = look();

        if (token.token == TokenType::t_NEWLINE) {
            consume(TokenType::t_NEWLINE);
            continue;
        }


        std::cout << "Token: " << TokenMetadata::GetInstance().GetTokenName(token.token) << std::endl;
        std::cout << "--- Parsing Statement Start ---" << std::endl;
        stmts.push_back(parseStatement());
        std::cout << "--- Parsing Statement End ---" << std::endl;
        std::cout << "Parsed statement: \n" << stmts.back().toCode(0) << std::endl;

        consume();
    }
    
    return BlockNode(1, 1, stmts);
}

BlockNode Parser::parseBlock() {
    consume(optType(TokenType::t_NEWLINE));
    TokenData start_token = consume(optType(TokenType::t_INDENT));

    std::vector<Node> stmts;
    while (more() && look().token != TokenType::t_DEDENT) {
        if (look().token == TokenType::t_NEWLINE) {
            consume(optType(TokenType::t_NEWLINE));
            continue;
        }
        stmts.push_back(parseStatement());
    }

    consume(optType(TokenType::t_DEDENT));
    return BlockNode(start_token.lineno, start_token.col, stmts);
}

Node Parser::parseStatement() {
    TokenData token = look();
    TokenData start_token;

    switch (token.token) {
        case t_IDENT:
            return parseAssignmentOrExpression();
        case t_BREAK:
            start_token = consume();
            return BreakNode(start_token.lineno, start_token.col);
        case t_CONTINUE:
            start_token = consume();
            return ContinueNode(start_token.lineno, start_token.col);
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

Node Parser::parseIdentifier() {
    TokenData token = consume(optType(TokenType::t_IDENT));
    Node node = VarNode(token.lineno, token.col, token.text);

    while (true) {
        token = look();
        if (token.token == TokenType::t_LPAREN) {
            node = parseCall(node);
        } else if (token.token == TokenType::t_LBRACK) {
            node = parseIndexOrSlice(node);
        } else if (token.token == TokenType::t_DOT) {
            node = parsePropertyAccess(node);
        } else {
            break;
        }
    }

    return node;
}

CallNode Parser::parseCall(Node& left) {
    consume(optType(TokenType::t_LPAREN));
    std::vector<Node> args = parseExpressionList(TokenType::t_RPAREN);
    consume(optType(TokenType::t_RPAREN));
    return CallNode(left.lineno, left.col, left, args);
}

Node Parser::parseIndexOrSlice(Node& left) {
    consume(optType(TokenType::t_LBRACK));

    std::optional<Node> start = std::nullopt;
    std::optional<Node> end = std::nullopt;
    std::optional<Node> step = std::nullopt;

    TokenData token = look();
    if (token.token != TokenType::t_COLON) {
        start = parseExpression();
    }

    if (look().token == TokenType::t_RBRACK) {
        if (start) {
            consume(optType(TokenType::t_RBRACK));
            return IndexNode(left.lineno, left.col, left, start.value());
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
    return SliceNode(left.lineno, left.col, left, start, end, step);
}

PropertyAccessNode Parser::parsePropertyAccess(Node& left) {
    consume(optType(TokenType::t_DOT));
    std::string right = consume(optType(TokenType::t_IDENT)).text;
    return PropertyAccessNode(left.lineno, left.col, left, right);
}

std::vector<Node> Parser::parseExpressionList(TokenType end_token) {
    ignoreWhitespace(true);

    std::vector<Node> args;
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

Node Parser::parseAssignmentOrExpression() {
    static std::set<TokenType> ASSIGNMENT_OPERATORS {
        TokenType::t_ASSIGN, TokenType::t_PLUS_ASSIGN,
        TokenType::t_MINUS_ASSIGN, TokenType::t_STAR_ASSIGN,
        TokenType::t_SLASH_ASSIGN, TokenType::t_PERCENT_ASSIGN
    };

    push();
    
    consume();
    std::optional<TokenData> next_token = peek();
    bool is_assignment = false;

    while (next_token) {
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

AssignNode Parser::parseAssignment() {
    Node left = parseIdentifier();
    TokenType op = consume().token;
    Node right = parseExpression();

    return AssignNode(left.lineno, left.col, left, right, op);
}

Node Parser::parseExpression() {
    return parseTernary();
}

Node Parser::parseTernary() {
    Node left = parseBooleanOps();
    while (more() && look().token == TokenType::t_IF) {
        consume(optType(TokenType::t_IF));
        Node cond = parseBooleanOps();
        consume(optType(TokenType::t_ELSE));
        Node right = parseExpression();
        left = TernaryNode(left.lineno, left.col, cond, left, right);
    }
    return left;
}

Node Parser::parseBooleanOps() {
    static std::set<TokenType> BOOLEAN_OPS {
        TokenType::t_AND, TokenType::t_OR
    };
    
    Node left = parseBooleanNot();
    while (more() && BOOLEAN_OPS.find(look().token) != BOOLEAN_OPS.end()) {
        TokenType op = consume().token;
        Node right = parseBooleanNot();
        left = BinaryOpNode(left.lineno, left.col, left, right, op);
    }
    return left;
}

Node Parser::parseBooleanNot() {
    if (more() && look().token == TokenType::t_NOT) {
        TokenData token = consume();
        Node operand = parseComparison();
        return UnaryOpNode(token.lineno, token.col, operand, TokenType::t_NOT);
    }
    return parseComparison();
}

Node Parser::parseComparison() {
    static std::set<TokenType> COMPARISON_OPS {
        TokenType::t_EQUAL, TokenType::t_NEQUAL,
        TokenType::t_LESS, TokenType::t_LESS_EQUAL,
        TokenType::t_GREATER, TokenType::t_GREATER_EQUAL
    };

    Node left = parseAdditive();
    while (more() && COMPARISON_OPS.find(look().token) != COMPARISON_OPS.end()) {
        TokenType op = consume().token;
        Node right = parseAdditive();
        left = BinaryOpNode(left.lineno, left.col, left, right, op);
    }
    return left;
}

Node Parser::parseAdditive() {
    static std::set<TokenType> ADDITIVE_OPS {
        TokenType::t_PLUS, TokenType::t_MINUS, TokenType::t_PERCENT
    };

    Node left = parseMultiplicative();
    while (more() && ADDITIVE_OPS.find(look().token) != ADDITIVE_OPS.end()) {
        TokenType op = consume().token;
        Node right = parseMultiplicative();
        left = BinaryOpNode(left.lineno, left.col, left, right, op);
    }
    return left;
}

Node Parser::parseMultiplicative() {
    static std::set<TokenType> MULTIPLICATIVE_OPS {
        TokenType::t_STAR, TokenType::t_SLASH
    };

    Node left = parseUnary();
    while (more() && MULTIPLICATIVE_OPS.find(look().token) != MULTIPLICATIVE_OPS.end()) {
        TokenType op = consume().token;
        Node right = parseUnary();
        left = BinaryOpNode(left.lineno, left.col, left, right, op);
    }
    return left;
}

Node Parser::parseUnary() {
    if (more()) {
        TokenData token = look();
        TokenType op = token.token;
        if (op == TokenType::t_PLUS || op == TokenType::t_MINUS) {
            consume();
            Node right = parseAtom();
            return UnaryOpNode(token.lineno, token.col, right, op);
        }
    }
    return parseAtom();
}

Node Parser::parseAtom() {
    TokenData token = look();
    std::string unescaped;
    Node node = Node(0, 0, NodeType::n_RETURN);

    switch (token.token) {
        case TokenType::t_NUMBER:
            consume();
            if (token.text.find('.') != std::string::npos) {
                return FloatNode(token.lineno, token.col, std::stof(token.text));
            }
            return IntNode(token.lineno, token.col, std::stoi(token.text));
        case TokenType::t_STRING:
            consume();
            unescaped = unescapeString(token.text.substr(1, token.text.size() - 2));
            return StringNode(token.lineno, token.col, unescaped);
        case TokenType::t_TRUE:
        case TokenType::t_FALSE:
            consume();
            return BoolNode(token.lineno, token.col, token.token == TokenType::t_TRUE);
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

Node Parser::parseListLiteral() {
    TokenData start_node = consume(optType(TokenType::t_LBRACK));

    ignoreWhitespace(true);
    std::vector<Node> args = parseExpressionList(TokenType::t_RBRACK);
    ignoreWhitespace(false);

    consume(optType(TokenType::t_RBRACK));
    return ListLiteralNode(start_node.lineno, start_node.col, args);
}

Node Parser::parseDictOrSetLiteral() {
    TokenData start_node = consume(optType(TokenType::t_LBRACE));
    ignoreWhitespace(true);

    push();

    std::vector<DictPair> pairs;

    if (look().token == TokenType::t_RBRACE) {
        consume(optType(TokenType::t_RBRACE));
        ignoreWhitespace(false);
        pop(false);
        return DictLiteralNode(start_node.lineno, start_node.col, pairs);
    }

    Node left = parseExpression();
    if (look().token != TokenType::t_COLON) {
        pop();
        std::vector<Node> elems = parseExpressionList(TokenType::t_RBRACE);
        consume(optType(TokenType::t_RBRACE));
        return SetLiteralNode(start_node.lineno, start_node.col, elems);
    }

    pop(false);

    Node right = parseExpression();
    pairs.push_back(std::make_pair(left, right));

    while (look().token == TokenType::t_COMMA) {
        consume(optType(TokenType::t_COMMA));
        if (look().token == TokenType::t_RBRACE) {
            break;
        }
        left = parseExpression();
        consume(optType(TokenType::t_COLON));
        right = parseExpression();
        pairs.push_back(std::make_pair(left, right));
    }

    consume(optType(TokenType::t_RBRACE));
    ignoreWhitespace(false);

    return DictLiteralNode(start_node.lineno, start_node.col, pairs);
}

IfNode Parser::parseIf() {
    TokenData start_token = consume(optType(TokenType::t_IF));
    Node cond = parseExpression();
    consume(optType(TokenType::t_COLON));
    BlockNode block = parseBlock();

    CondBlock if_block = std::make_pair(cond, block);
    
    std::vector<CondBlock> elif_blocks;
    while (more() && look().token == TokenType::t_ELIF) {
        consume(optType(TokenType::t_ELIF));
        cond = parseExpression();
        consume(optType(TokenType::t_COLON));
        block = parseBlock();
        elif_blocks.push_back(std::make_pair(cond, block));
    }

    std::optional<BlockNode> else_block = std::nullopt;
    if (more() && look().token == TokenType::t_ELSE) {
        consume(optType(TokenType::t_ELSE));
        consume(optType(TokenType::t_COLON));
        else_block = std::optional<BlockNode>(parseBlock());
    }

    return IfNode(start_token.lineno, start_token.col, if_block, elif_blocks, else_block);
}

WhileNode Parser::parseWhile() {
    TokenData start_token = consume(optType(TokenType::t_WHILE));
    Node cond = parseExpression();
    consume(optType(TokenType::t_COLON));
    BlockNode block = parseBlock();

    return WhileNode(start_token.lineno, start_token.col, cond, block);
}

ForNode Parser::parseFor() {
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
    Node iterable = parseExpression();
    consume(optType(TokenType::t_COLON));
    BlockNode block = parseBlock();

    return ForNode(start_token.lineno, start_token.col, vars, iterable, block);
}

FunctionDefinitionNode Parser::parseDef() {
    TokenData start_token = consume(optType(TokenType::t_DEF));
    std::string name = consume(optType(TokenType::t_IDENT)).text;
    consume(optType(TokenType::t_LPAREN));
    std::vector<std::string> args = parseIdentifierList();
    consume(optType(TokenType::t_RPAREN));
    consume(optType(TokenType::t_COLON));
    BlockNode block = parseBlock();

    return FunctionDefinitionNode(start_token.lineno, start_token.col, name, args, block);
}

ReturnNode Parser::parseReturn() {
    TokenData start_token = consume(optType(TokenType::t_RETURN));

    if (look().token == TokenType::t_NEWLINE) {
        return ReturnNode(start_token.lineno, start_token.col, std::nullopt);
    }

    Node right = parseExpression();

    return ReturnNode(start_token.lineno, start_token.col, std::optional<Node>(right));
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