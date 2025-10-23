#pragma once

#include <array>
#include <vector>
#include <map>
#include <string>

enum BLToken {
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
  BREAK,
  CONTINUE,
  RETURN,
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
  PERCENT_ASSIGN
};
constexpr size_t TOKEN_COUNT = static_cast<size_t>(BLToken::PERCENT_ASSIGN) + 1;
typedef std::pair<BLToken, std::string> RegexPair;

class TokenMetadata {
  public:
    static TokenMetadata& GetInstance();
    const std::string& GetTokenString(BLToken token) const;
    const std::string& GetTokenName(BLToken token) const;
    BLToken GetTokenFromName(const std::string &name) const;
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

    const std::map<std::string, BLToken> token_name_map{
      {"INVALID_TOKEN", BLToken::INVALID_TOKEN},
      {"IDENT", BLToken::IDENT},
      {"NUMBER", BLToken::NUMBER},
      {"STRING", BLToken::STRING},
      {"INDENT", BLToken::INDENT},
      {"DEDENT", BLToken::DEDENT},
      {"NEWLINE", BLToken::NEWLINE},
      {"END_OF_FILE", BLToken::END_OF_FILE},
      {"WHITESPACE", BLToken::WHITESPACE},
      {"IF", BLToken::IF},
      {"ELSE", BLToken::ELSE},
      {"ELIF", BLToken::ELIF},
      {"WHILE", BLToken::WHILE},
      {"FOR", BLToken::FOR},
      {"IN", BLToken::IN},
      {"DEF", BLToken::DEF},
      {"BREAK", BLToken::BREAK},
      {"CONTINUE", BLToken::CONTINUE},
      {"RETURN", BLToken::RETURN},
      {"AND", BLToken::AND},
      {"OR", BLToken::OR},
      {"NOT", BLToken::NOT},
      {"TRUE", BLToken::TRUE},
      {"FALSE", BLToken::FALSE},
      {"LPAREN", BLToken::LPAREN},
      {"RPAREN", BLToken::RPAREN},
      {"LBRACK", BLToken::LBRACK},
      {"RBRACK", BLToken::RBRACK},
      {"LBRACE", BLToken::LBRACE},
      {"RBRACE", BLToken::RBRACE},
      {"COLON", BLToken::COLON},
      {"COMMA", BLToken::COMMA},
      {"DOT", BLToken::DOT},
      {"COMMENT", BLToken::COMMENT},
      {"PLUS", BLToken::PLUS},
      {"MINUS", BLToken::MINUS},
      {"STAR", BLToken::STAR},
      {"SLASH", BLToken::SLASH},
      {"PERCENT", BLToken::PERCENT},
      {"EQUAL", BLToken::EQUAL},
      {"NEQUAL", BLToken::NEQUAL},
      {"LESS_EQUAL", BLToken::LESS_EQUAL},
      {"GREATER_EQUAL", BLToken::GREATER_EQUAL},
      {"LESS", BLToken::LESS},
      {"GREATER", BLToken::GREATER},
      {"ASSIGN", BLToken::ASSIGN},
      {"PLUS_ASSIGN", BLToken::PLUS_ASSIGN},
      {"MINUS_ASSIGN", BLToken::MINUS_ASSIGN},
      {"STAR_ASSIGN", BLToken::STAR_ASSIGN},
      {"SLASH_ASSIGN", BLToken::SLASH_ASSIGN},
      {"PERCENT_ASSIGN", BLToken::PERCENT_ASSIGN},
    };
    
    const std::vector<RegexPair> token_regexes{
      RegexPair{BLToken::NUMBER,          R"(\d+(?:\.\d+)?)"},
      RegexPair{BLToken::DEF,             R"(\bdef\b)"},
      RegexPair{BLToken::IF,              R"(\bif\b)"},
      RegexPair{BLToken::ELSE,            R"(\belse\b)"},
      RegexPair{BLToken::ELIF,            R"(\belif\b)"},
      RegexPair{BLToken::WHILE,           R"(\bwhile\b)"},
      RegexPair{BLToken::FOR,             R"(\bfor\b)"},
      RegexPair{BLToken::IN,              R"(\bin\b)"},
      RegexPair{BLToken::BREAK,           R"(\bbreak\b)"},
      RegexPair{BLToken::CONTINUE,        R"(\bcontinue\b)"},
      RegexPair{BLToken::RETURN,          R"(\breturn\b)"},
      RegexPair{BLToken::AND,             R"(\band\b)"},
      RegexPair{BLToken::OR,              R"(\bor\b)"},
      RegexPair{BLToken::NOT,             R"(\bnot\b)"},
      RegexPair{BLToken::TRUE,            R"(\bTrue\b)"},
      RegexPair{BLToken::FALSE,           R"(\bFalse\b)"},
      RegexPair{BLToken::STRING,          R"("[^"]*"|'[^']*')"},
      RegexPair{BLToken::IDENT,           R"([A-Za-z_][A-Za-z_0-9]*)"},
      RegexPair{BLToken::PLUS_ASSIGN,     R"(\+=)"},
      RegexPair{BLToken::MINUS_ASSIGN,    R"(-=)"},
      RegexPair{BLToken::STAR_ASSIGN,     R"(\*=)"},
      RegexPair{BLToken::SLASH_ASSIGN,    R"(/=)"},
      RegexPair{BLToken::PERCENT_ASSIGN,  R"(%=)"},
      RegexPair{BLToken::EQUAL,           R"(==)"},
      RegexPair{BLToken::NEQUAL,          R"(!=)"},
      RegexPair{BLToken::LESS_EQUAL,      R"(<=)"},
      RegexPair{BLToken::GREATER_EQUAL,   R"(>=)"},
      RegexPair{BLToken::PLUS,            R"(\+)"},
      RegexPair{BLToken::MINUS,           R"(-)"},
      RegexPair{BLToken::STAR,            R"(\*)"},
      RegexPair{BLToken::SLASH,           R"(/)"},
      RegexPair{BLToken::PERCENT,         R"(%)"},
      RegexPair{BLToken::LESS,            R"(<)"},
      RegexPair{BLToken::GREATER,         R"(>)"},
      RegexPair{BLToken::ASSIGN,          R"(=)"},
      RegexPair{BLToken::LPAREN,          R"(\()"},
      RegexPair{BLToken::RPAREN,          R"(\))"},
      RegexPair{BLToken::LBRACK,          R"(\[)"},
      RegexPair{BLToken::RBRACK,          R"(\])"},
      RegexPair{BLToken::LBRACE,          R"(\{)"},
      RegexPair{BLToken::RBRACE,          R"(\})"},
      RegexPair{BLToken::COMMA,           R"(,)"},
      RegexPair{BLToken::COLON,           R"(:)"},
      RegexPair{BLToken::DOT,             R"(\.)"},
      RegexPair{BLToken::NEWLINE,         R"(\n)"},
      RegexPair{BLToken::WHITESPACE,      R"([ \t]+)"},
      RegexPair{BLToken::INVALID_TOKEN,   R"(.+)"}
    };
    
  public:
    TokenMetadata(TokenMetadata const&) = delete;
    void operator=(TokenMetadata const&) = delete;
  };