from enum import Enum

class BLToken(Enum):
    IDENT = 1
    NUMBER = 2
    STRING = 3
    ASSIGN = 4
    PLUS = 5
    MINUS = 6
    STAR = 7
    SLASH = 8
    IF = 9
    ELSE = 10
    ELIF = 11
    WHILE = 12
    FOR = 13
    IN = 14
    EQUAL = 15
    NEQUAL = 16
    LESS_EQUAL = 17
    GREATER_EQUAL = 18
    LESS = 19
    GREATER = 20
    LPAREN = 21
    RPAREN = 22
    LBRACK = 23
    RBRACK = 24
    LBRACE = 25
    RBRACE = 26
    COLON = 27
    COMMA = 28
    INDENT = 29
    DEDENT = 30
    DEF = 31
    PLUS_ASSIGN = 32
    MINUS_ASSIGN = 33
    STAR_ASSIGN = 34
    SLASH_ASSIGN = 35
    NEWLINE = 40
    EOF = 41
    WHITESPACE = 99

    def name(self):
        return self._name_
    
TOKEN_STRING_MAP = {
    BLToken.ASSIGN: "=",
    BLToken.PLUS: "+",
    BLToken.MINUS: "-",
    BLToken.STAR: "*",
    BLToken.SLASH: "/",
    BLToken.IF: "if",
    BLToken.ELSE: "else",
    BLToken.ELIF: "elif",
    BLToken.WHILE: "while",
    BLToken.FOR: "for",
    BLToken.IN: "in",
    BLToken.EQUAL: "==",
    BLToken.NEQUAL: "!=",
    BLToken.LESS_EQUAL: "<=",
    BLToken.GREATER_EQUAL: ">=",
    BLToken.LESS: "<=",
    BLToken.GREATER: ">",
    BLToken.LPAREN: "(",
    BLToken.RPAREN: ")",
    BLToken.LBRACK: "[",
    BLToken.RBRACK: "]",
    BLToken.LBRACE: "{",
    BLToken.RBRACE: "}",
    BLToken.COLON: ":",
    BLToken.COMMA: ",",
    BLToken.INDENT: "\t",
    BLToken.DEDENT: "",
    BLToken.DEF: "def",
    BLToken.PLUS_ASSIGN: "+=",
    BLToken.MINUS_ASSIGN: "-=",
    BLToken.STAR_ASSIGN: "*=",
    BLToken.SLASH_ASSIGN: "/=",
    BLToken.NEWLINE: "\n",
}