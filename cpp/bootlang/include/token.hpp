#pragma once

#include <array>
#include <vector>
#include <map>
#include <string>

enum TokenType {
  t_INVALID_TOKEN,
  t_IDENT,
  t_NUMBER,
  t_STRING,
  t_INDENT,
  t_DEDENT,
  t_NEWLINE,
  t_END_OF_FILE,
  t_WHITESPACE,
  t_IF,
  t_ELSE,
  t_ELIF,
  t_WHILE,
  t_FOR,
  t_IN,
  t_DEF,
  t_BREAK,
  t_CONTINUE,
  t_RETURN,
  t_AND,
  t_OR,
  t_NOT,
  t_TRUE,
  t_FALSE,
  t_LPAREN,
  t_RPAREN,
  t_LBRACK,
  t_RBRACK,
  t_LBRACE,
  t_RBRACE,
  t_COLON,
  t_COMMA,
  t_DOT,
  t_COMMENT,
  t_PLUS,
  t_MINUS,
  t_STAR,
  t_SLASH,
  t_PERCENT,
  t_EQUAL,
  t_NEQUAL,
  t_LESS_EQUAL,
  t_GREATER_EQUAL,
  t_LESS,
  t_GREATER,
  t_ASSIGN,
  t_PLUS_ASSIGN,
  t_MINUS_ASSIGN,
  t_STAR_ASSIGN,
  t_SLASH_ASSIGN,
  t_PERCENT_ASSIGN
};
constexpr size_t TOKEN_COUNT = static_cast<size_t>(TokenType::t_PERCENT_ASSIGN) + 1;
typedef std::pair<TokenType, std::string> RegexPair;

class TokenMetadata {
  public:
    static TokenMetadata& GetInstance();
    const std::string& GetTokenString(TokenType token) const;
    const std::string& GetTokenName(TokenType token) const;
    TokenType GetTokenFromName(const std::string &name) const;
    const std::vector<RegexPair> &GetTokenRegexes() const;
    
  private:
    TokenMetadata() = default;

    const std::array<std::string, TOKEN_COUNT> token_strings {
      "INVALID_TOKEN",
      "", "", "",     // IDENT, NUMBER, STRING
      "\t", "", "\n", "", "", // INDENT, DEDENT, NEWLINE, END_OF_FILE, WHITESPACE
      "if", "else", "elif", "while", "for", "in", "def", "break",
      "continue", "return", "and", "or", "not ", "True", "False",
      "(", ")", "[", "]", "{", "}", ":", ",", ".", "#",
      "+", "-", "*", "/", "%", "==", "!=", "<=", ">=", "<", ">", 
      "=", "+=", "-=", "*=", "/=", "%="
    };

    const std::array<std::string, TOKEN_COUNT> token_names{
      "INVALID_TOKEN",
      "IDENT", "NUMBER", "STRING",
      "INDENT", "DEDENT", "NEWLINE", "END_OF_FILE", "WHITESPACE",
      "IF", "ELSE", "ELIF", "WHILE", "FOR",
      "IN", "DEF", "BREAK", "CONTINUE", "RETURN",
      "AND", "OR", "NOT", "TRUE", "FALSE",
      "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE",
      "COLON", "COMMA", "DOT", "COMMENT",
      "PLUS", "MINUS", "STAR", "SLASH", "PERCENT",
      "EQUAL", "NEQUAL", "LESS_EQUAL", "GREATER_EQUAL", "LESS","GREATER",
      "ASSIGN", "PLUS_ASSIGN", "MINUS_ASSIGN", "STAR_ASSIGN", "SLASH_ASSIGN", "PERCENT_ASSIGN"
    };

    const std::map<std::string, TokenType> token_name_map{
      {"INVALID_TOKEN", TokenType::t_INVALID_TOKEN},
      {"IDENT", TokenType::t_IDENT},
      {"NUMBER", TokenType::t_NUMBER},
      {"STRING", TokenType::t_STRING},
      {"INDENT", TokenType::t_INDENT},
      {"DEDENT", TokenType::t_DEDENT},
      {"NEWLINE", TokenType::t_NEWLINE},
      {"END_OF_FILE", TokenType::t_END_OF_FILE},
      {"WHITESPACE", TokenType::t_WHITESPACE},
      {"IF", TokenType::t_IF},
      {"ELSE", TokenType::t_ELSE},
      {"ELIF", TokenType::t_ELIF},
      {"WHILE", TokenType::t_WHILE},
      {"FOR", TokenType::t_FOR},
      {"IN", TokenType::t_IN},
      {"DEF", TokenType::t_DEF},
      {"BREAK", TokenType::t_BREAK},
      {"CONTINUE", TokenType::t_CONTINUE},
      {"RETURN", TokenType::t_RETURN},
      {"AND", TokenType::t_AND},
      {"OR", TokenType::t_OR},
      {"NOT", TokenType::t_NOT},
      {"TRUE", TokenType::t_TRUE},
      {"FALSE", TokenType::t_FALSE},
      {"LPAREN", TokenType::t_LPAREN},
      {"RPAREN", TokenType::t_RPAREN},
      {"LBRACK", TokenType::t_LBRACK},
      {"RBRACK", TokenType::t_RBRACK},
      {"LBRACE", TokenType::t_LBRACE},
      {"RBRACE", TokenType::t_RBRACE},
      {"COLON", TokenType::t_COLON},
      {"COMMA", TokenType::t_COMMA},
      {"DOT", TokenType::t_DOT},
      {"COMMENT", TokenType::t_COMMENT},
      {"PLUS", TokenType::t_PLUS},
      {"MINUS", TokenType::t_MINUS},
      {"STAR", TokenType::t_STAR},
      {"SLASH", TokenType::t_SLASH},
      {"PERCENT", TokenType::t_PERCENT},
      {"EQUAL", TokenType::t_EQUAL},
      {"NEQUAL", TokenType::t_NEQUAL},
      {"LESS_EQUAL", TokenType::t_LESS_EQUAL},
      {"GREATER_EQUAL", TokenType::t_GREATER_EQUAL},
      {"LESS", TokenType::t_LESS},
      {"GREATER", TokenType::t_GREATER},
      {"ASSIGN", TokenType::t_ASSIGN},
      {"PLUS_ASSIGN", TokenType::t_PLUS_ASSIGN},
      {"MINUS_ASSIGN", TokenType::t_MINUS_ASSIGN},
      {"STAR_ASSIGN", TokenType::t_STAR_ASSIGN},
      {"SLASH_ASSIGN", TokenType::t_SLASH_ASSIGN},
      {"PERCENT_ASSIGN", TokenType::t_PERCENT_ASSIGN},
    };
    
    const std::vector<RegexPair> token_regexes{
      RegexPair{TokenType::t_NUMBER,          R"(\d+(?:\.\d+)?)"},
      RegexPair{TokenType::t_DEF,             R"(\bdef\b)"},
      RegexPair{TokenType::t_IF,              R"(\bif\b)"},
      RegexPair{TokenType::t_ELSE,            R"(\belse\b)"},
      RegexPair{TokenType::t_ELIF,            R"(\belif\b)"},
      RegexPair{TokenType::t_WHILE,           R"(\bwhile\b)"},
      RegexPair{TokenType::t_FOR,             R"(\bfor\b)"},
      RegexPair{TokenType::t_IN,              R"(\bin\b)"},
      RegexPair{TokenType::t_BREAK,           R"(\bbreak\b)"},
      RegexPair{TokenType::t_CONTINUE,        R"(\bcontinue\b)"},
      RegexPair{TokenType::t_RETURN,          R"(\breturn\b)"},
      RegexPair{TokenType::t_AND,             R"(\band\b)"},
      RegexPair{TokenType::t_OR,              R"(\bor\b)"},
      RegexPair{TokenType::t_NOT,             R"(\bnot\b)"},
      RegexPair{TokenType::t_TRUE,            R"(\bTrue\b)"},
      RegexPair{TokenType::t_FALSE,           R"(\bFalse\b)"},
      RegexPair{TokenType::t_STRING,          R"("[^"]*"|'[^']*')"},
      RegexPair{TokenType::t_IDENT,           R"([A-Za-z_][A-Za-z_0-9]*)"},
      RegexPair{TokenType::t_PLUS_ASSIGN,     R"(\+=)"},
      RegexPair{TokenType::t_MINUS_ASSIGN,    R"(-=)"},
      RegexPair{TokenType::t_STAR_ASSIGN,     R"(\*=)"},
      RegexPair{TokenType::t_SLASH_ASSIGN,    R"(/=)"},
      RegexPair{TokenType::t_PERCENT_ASSIGN,  R"(%=)"},
      RegexPair{TokenType::t_EQUAL,           R"(==)"},
      RegexPair{TokenType::t_NEQUAL,          R"(!=)"},
      RegexPair{TokenType::t_LESS_EQUAL,      R"(<=)"},
      RegexPair{TokenType::t_GREATER_EQUAL,   R"(>=)"},
      RegexPair{TokenType::t_PLUS,            R"(\+)"},
      RegexPair{TokenType::t_MINUS,           R"(-)"},
      RegexPair{TokenType::t_STAR,            R"(\*)"},
      RegexPair{TokenType::t_SLASH,           R"(/)"},
      RegexPair{TokenType::t_PERCENT,         R"(%)"},
      RegexPair{TokenType::t_LESS,            R"(<)"},
      RegexPair{TokenType::t_GREATER,         R"(>)"},
      RegexPair{TokenType::t_ASSIGN,          R"(=)"},
      RegexPair{TokenType::t_LPAREN,          R"(\()"},
      RegexPair{TokenType::t_RPAREN,          R"(\))"},
      RegexPair{TokenType::t_LBRACK,          R"(\[)"},
      RegexPair{TokenType::t_RBRACK,          R"(\])"},
      RegexPair{TokenType::t_LBRACE,          R"(\{)"},
      RegexPair{TokenType::t_RBRACE,          R"(\})"},
      RegexPair{TokenType::t_COMMA,           R"(,)"},
      RegexPair{TokenType::t_COLON,           R"(:)"},
      RegexPair{TokenType::t_DOT,             R"(\.)"},
      RegexPair{TokenType::t_NEWLINE,         R"(\n)"},
      RegexPair{TokenType::t_WHITESPACE,      R"([ \t]+)"},
      RegexPair{TokenType::t_INVALID_TOKEN,   R"(.+)"}
    };
    
  public:
    TokenMetadata(TokenMetadata const&) = delete;
    void operator=(TokenMetadata const&) = delete;
  };