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
    return token;
}

BlockNode Parser::parse() {
    return BlockNode(1, 1, std::vector<Node>{});
}