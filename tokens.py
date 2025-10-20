from enum import Enum

class BLToken(Enum):
    IDENT = 1
    NUMBER = 2
    STRING = 3
    INDENT = 51
    DEDENT = 52
    NEWLINE = 53
    EOF = 54
    WHITESPACE = 55
    IF = 101
    ELSE = 102
    ELIF = 103
    WHILE = 104
    FOR = 105
    IN = 106
    DEF = 107
    BREAK = 108
    CONTINUE = 109
    RETURN = 110
    AND = 111
    OR = 112
    NOT = 113
    TRUE = 114
    FALSE = 115
    LPAREN = 151
    RPAREN = 152
    LBRACK = 153
    RBRACK = 154
    LBRACE = 155
    RBRACE = 156
    COLON = 157
    COMMA = 158
    DOT = 159
    COMMENT = 160
    PLUS = 201
    MINUS = 202
    STAR = 203
    SLASH = 204
    PERCENT = 205
    EQUAL = 251
    NEQUAL = 252
    LESS_EQUAL = 253
    GREATER_EQUAL = 254
    LESS = 255
    GREATER = 256
    ASSIGN = 301
    PLUS_ASSIGN = 302
    MINUS_ASSIGN = 303
    STAR_ASSIGN = 304
    SLASH_ASSIGN = 305
    PERCENT_ASSIGN = 306
    INVALID_TOKEN = 999

    def name(self):
        return self._name_
    
TOKEN_STRING_MAP = {
    BLToken.ASSIGN: "=",
    BLToken.PLUS: "+",
    BLToken.MINUS: "-",
    BLToken.STAR: "*",
    BLToken.SLASH: "/",
    BLToken.PERCENT: "%",
    BLToken.IF: "if",
    BLToken.ELSE: "else",
    BLToken.ELIF: "elif",
    BLToken.WHILE: "while",
    BLToken.FOR: "for",
    BLToken.IN: "in",
    BLToken.BREAK: "break",
    BLToken.CONTINUE: "continue",
    BLToken.RETURN: "return",
    BLToken.AND: "and",
    BLToken.OR: "or",
    BLToken.NOT: "not ",
    BLToken.TRUE: "true",
    BLToken.FALSE: "false",
    BLToken.EQUAL: "==",
    BLToken.NEQUAL: "!=",
    BLToken.LESS_EQUAL: "<=",
    BLToken.GREATER_EQUAL: ">=",
    BLToken.LESS: "<",
    BLToken.GREATER: ">",
    BLToken.LPAREN: "(",
    BLToken.RPAREN: ")",
    BLToken.LBRACK: "[",
    BLToken.RBRACK: "]",
    BLToken.LBRACE: "{",
    BLToken.RBRACE: "}",
    BLToken.COLON: ":",
    BLToken.COMMA: ",",
    BLToken.DOT: ".",
    BLToken.COMMENT: "#",
    BLToken.INDENT: "\t",
    BLToken.DEDENT: "",
    BLToken.DEF: "def",
    BLToken.PLUS_ASSIGN: "+=",
    BLToken.MINUS_ASSIGN: "-=",
    BLToken.STAR_ASSIGN: "*=",
    BLToken.SLASH_ASSIGN: "/=",
    BLToken.PERCENT_ASSIGN: "%=",
    BLToken.NEWLINE: "\n",
}