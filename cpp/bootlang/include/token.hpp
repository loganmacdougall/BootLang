#pragma once

#include <array>
#include <vector>
#include <map>
#include <string>

namespace Token {
  enum Type {
    INVALID_TOKEN,
    IDENT,
    NUMBER,
    STRING,
    INDENT,
    DEDENT,
    NEWLINE,
    END_OF_FILE,
    WHITESPACE,
    IF,
    ELSE,
    ELIF,
    WHILE,
    FOR,
    IN,
    DEF,
    CLASS,
    BREAK,
    CONTINUE,
    RETURN,
    YIELD,
    AND,
    OR,
    NOT,
    TRUE,
    FALSE,
    LPAREN,
    RPAREN,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE,
    COLON,
    COMMA,
    DOT,
    COMMENT,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    DOUBLE_SLASH,
    PERCENT,
    EQUAL,
    NEQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    LESS,
    GREATER,
    ASSIGN,
    PLUS_ASSIGN,
    MINUS_ASSIGN,
    STAR_ASSIGN,
    SLASH_ASSIGN,
    DOUBLE_SLASH_ASSIGN,
    PERCENT_ASSIGN
  };

  const std::string& typeString(Token::Type type);
  static const size_t TOKEN_COUNT = static_cast<size_t>(Type::PERCENT_ASSIGN) + 1;
};

class TokenMetadata {
  public:
    typedef std::pair<Token::Type, std::string> RegexPair;
    
  public:
    static const TokenMetadata& GetInstance();
    const std::string& GetTokenString(Token::Type token) const;
    const std::string& GetTokenName(Token::Type token) const;
    Token::Type GetTokenFromName(const std::string &name) const;
    const std::vector<RegexPair> &GetTokenRegexes() const;
    
  private:
    TokenMetadata() = default;

    const std::array<std::string, Token::TOKEN_COUNT> token_strings {
      "INVALID_TOKEN",
      "", "", "",     // IDENT, NUMBER, STRING
      "\t", "", "\n", "", "", // INDENT, DEDENT, NEWLINE, END_OF_FILE, WHITESPACE
      "if", "else", "elif", "while", "for", "in", "def", "class", "break",
      "continue", "return", "yield", "and", "or", "not ", "True", "False",
      "(", ")", "[", "]", "{", "}", ":", ",", ".", "#",
      "+", "-", "*", "/", "//", "%", "==", "!=", "<=", ">=", "<", ">", 
      "=", "+=", "-=", "*=", "/=", "//=", "%="
    };

    const std::array<std::string, Token::TOKEN_COUNT> token_names{
      "INVALID_TOKEN",
      "IDENT", "NUMBER", "STRING",
      "INDENT", "DEDENT", "NEWLINE", "END_OF_FILE", "WHITESPACE",
      "IF", "ELSE", "ELIF", "WHILE", "FOR",
      "IN", "DEF", "CLASS", "BREAK", "CONTINUE", "RETURN", "YIELD",
      "AND", "OR", "NOT", "TRUE", "FALSE",
      "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE",
      "COLON", "COMMA", "DOT", "COMMENT",
      "PLUS", "MINUS", "STAR", "SLASH", "DOUBLE_SLASH", "PERCENT",
      "EQUAL", "NEQUAL", "LESS_EQUAL", "GREATER_EQUAL", "LESS","GREATER",
      "ASSIGN", "PLUS_ASSIGN", "MINUS_ASSIGN", "STAR_ASSIGN", "SLASH_ASSIGN", "DOUBLE_SLASH_ASSIGN", "PERCENT_ASSIGN"
    };

    const std::map<std::string, Token::Type> token_name_map{
      {"INVALID_TOKEN", Token::Type::INVALID_TOKEN},
      {"IDENT", Token::Type::IDENT},
      {"NUMBER", Token::Type::NUMBER},
      {"STRING", Token::Type::STRING},
      {"INDENT", Token::Type::INDENT},
      {"DEDENT", Token::Type::DEDENT},
      {"NEWLINE", Token::Type::NEWLINE},
      {"END_OF_FILE", Token::Type::END_OF_FILE},
      {"WHITESPACE", Token::Type::WHITESPACE},
      {"IF", Token::Type::IF},
      {"ELSE", Token::Type::ELSE},
      {"ELIF", Token::Type::ELIF},
      {"WHILE", Token::Type::WHILE},
      {"FOR", Token::Type::FOR},
      {"IN", Token::Type::IN},
      {"DEF", Token::Type::DEF},
      {"CLASS", Token::Type::CLASS},
      {"BREAK", Token::Type::BREAK},
      {"CONTINUE", Token::Type::CONTINUE},
      {"RETURN", Token::Type::RETURN},
      {"YIELD", Token::Type::YIELD},
      {"AND", Token::Type::AND},
      {"OR", Token::Type::OR},
      {"NOT", Token::Type::NOT},
      {"TRUE", Token::Type::TRUE},
      {"FALSE", Token::Type::FALSE},
      {"LPAREN", Token::Type::LPAREN},
      {"RPAREN", Token::Type::RPAREN},
      {"LBRACK", Token::Type::LBRACK},
      {"RBRACK", Token::Type::RBRACK},
      {"LBRACE", Token::Type::LBRACE},
      {"RBRACE", Token::Type::RBRACE},
      {"COLON", Token::Type::COLON},
      {"COMMA", Token::Type::COMMA},
      {"DOT", Token::Type::DOT},
      {"COMMENT", Token::Type::COMMENT},
      {"PLUS", Token::Type::PLUS},
      {"MINUS", Token::Type::MINUS},
      {"STAR", Token::Type::STAR},
      {"SLASH", Token::Type::SLASH},
      {"DOUBLE_SLASH", Token::Type::DOUBLE_SLASH},
      {"PERCENT", Token::Type::PERCENT},
      {"EQUAL", Token::Type::EQUAL},
      {"NEQUAL", Token::Type::NEQUAL},
      {"LESS_EQUAL", Token::Type::LESS_EQUAL},
      {"GREATER_EQUAL", Token::Type::GREATER_EQUAL},
      {"LESS", Token::Type::LESS},
      {"GREATER", Token::Type::GREATER},
      {"ASSIGN", Token::Type::ASSIGN},
      {"PLUS_ASSIGN", Token::Type::PLUS_ASSIGN},
      {"MINUS_ASSIGN", Token::Type::MINUS_ASSIGN},
      {"STAR_ASSIGN", Token::Type::STAR_ASSIGN},
      {"SLASH_ASSIGN", Token::Type::SLASH_ASSIGN},
      {"DOUBLE_SLASH_ASSIGN", Token::Type::DOUBLE_SLASH_ASSIGN},
      {"PERCENT_ASSIGN", Token::Type::PERCENT_ASSIGN},
    };
    
    const std::vector<RegexPair> token_regexes{
      RegexPair{Token::Type::NUMBER,          R"(\d+(?:\.\d+)?)"},
      RegexPair{Token::Type::DEF,             R"(\bdef\b)"},
      RegexPair{Token::Type::CLASS,           R"(\bclass\b)"},
      RegexPair{Token::Type::IF,              R"(\bif\b)"},
      RegexPair{Token::Type::ELSE,            R"(\belse\b)"},
      RegexPair{Token::Type::ELIF,            R"(\belif\b)"},
      RegexPair{Token::Type::WHILE,           R"(\bwhile\b)"},
      RegexPair{Token::Type::FOR,             R"(\bfor\b)"},
      RegexPair{Token::Type::IN,              R"(\bin\b)"},
      RegexPair{Token::Type::BREAK,           R"(\bbreak\b)"},
      RegexPair{Token::Type::CONTINUE,        R"(\bcontinue\b)"},
      RegexPair{Token::Type::RETURN,          R"(\breturn\b)"},
      RegexPair{Token::Type::YIELD,           R"(\byield\b)"},
      RegexPair{Token::Type::AND,             R"(\band\b)"},
      RegexPair{Token::Type::OR,              R"(\bor\b)"},
      RegexPair{Token::Type::NOT,             R"(\bnot\b)"},
      RegexPair{Token::Type::TRUE,            R"(\bTrue\b)"},
      RegexPair{Token::Type::FALSE,           R"(\bFalse\b)"},
      RegexPair{Token::Type::STRING,          R"("[^"\n]*"|'[^'\n]*'|"""[^"]*"""|'''[^']*''')"},
      RegexPair{Token::Type::IDENT,           R"([A-Za-z_][A-Za-z_0-9]*)"},
      RegexPair{Token::Type::PLUS_ASSIGN,     R"(\+=)"},
      RegexPair{Token::Type::MINUS_ASSIGN,    R"(-=)"},
      RegexPair{Token::Type::STAR_ASSIGN,     R"(\*=)"},
      RegexPair{Token::Type::DOUBLE_SLASH_ASSIGN,    R"(//=)"},
      RegexPair{Token::Type::SLASH_ASSIGN,    R"(/=)"},
      RegexPair{Token::Type::PERCENT_ASSIGN,  R"(%=)"},
      RegexPair{Token::Type::EQUAL,           R"(==)"},
      RegexPair{Token::Type::NEQUAL,          R"(!=)"},
      RegexPair{Token::Type::LESS_EQUAL,      R"(<=)"},
      RegexPair{Token::Type::GREATER_EQUAL,   R"(>=)"},
      RegexPair{Token::Type::PLUS,            R"(\+)"},
      RegexPair{Token::Type::MINUS,           R"(-)"},
      RegexPair{Token::Type::STAR,            R"(\*)"},
      RegexPair{Token::Type::DOUBLE_SLASH,           R"(//)"},
      RegexPair{Token::Type::SLASH,           R"(/)"},
      RegexPair{Token::Type::PERCENT,         R"(%)"},
      RegexPair{Token::Type::LESS,            R"(<)"},
      RegexPair{Token::Type::GREATER,         R"(>)"},
      RegexPair{Token::Type::ASSIGN,          R"(=)"},
      RegexPair{Token::Type::LPAREN,          R"(\()"},
      RegexPair{Token::Type::RPAREN,          R"(\))"},
      RegexPair{Token::Type::LBRACK,          R"(\[)"},
      RegexPair{Token::Type::RBRACK,          R"(\])"},
      RegexPair{Token::Type::LBRACE,          R"(\{)"},
      RegexPair{Token::Type::RBRACE,          R"(\})"},
      RegexPair{Token::Type::COMMA,           R"(,)"},
      RegexPair{Token::Type::COLON,           R"(:)"},
      RegexPair{Token::Type::DOT,             R"(\.)"},
      RegexPair{Token::Type::NEWLINE,         R"(\n)"},
      RegexPair{Token::Type::WHITESPACE,      R"([ \t]+)"},
      RegexPair{Token::Type::COMMENT,         R"(#[^\n]*)"},
      RegexPair{Token::Type::INVALID_TOKEN,   R"(.+)"}
    };
    
  public:
    TokenMetadata(TokenMetadata const&) = delete;
    void operator=(TokenMetadata const&) = delete;
};