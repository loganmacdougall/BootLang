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
    NEWLINE = 30
    EOF = 31
    SKIP = 99

    def name(self):
        return self._name_