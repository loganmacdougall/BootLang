import re
from tokens import BLToken

SPACES_IN_TAB = 2

_token_patterns = [
    (BLToken.NUMBER, r'\d+(\.\d+)?'),
    (BLToken.DEF, r'def'),
    (BLToken.IF, r'if'),
    (BLToken.ELSE, r'else'),
    (BLToken.ELIF, r'elif'),
    (BLToken.WHILE, r'while'),
    (BLToken.FOR, r'for'),
    (BLToken.IN, r'in'),
    (BLToken.STRING, r'"[^"]*"'),
    (BLToken.IDENT, r'[A-Za-z_][A-Za-z_0-9]*'),
    (BLToken.PLUS_ASSIGN, r'\+='),
    (BLToken.MINUS_ASSIGN, r'-='),
    (BLToken.STAR_ASSIGN, r'\*='),
    (BLToken.SLASH_ASSIGN, r'/='),
    (BLToken.EQUAL, r'=='),
    (BLToken.NEQUAL, r'!='),
    (BLToken.LESS_EQUAL, r'<='),
    (BLToken.GREATER_EQUAL, r'>='),
    (BLToken.PLUS, r'\+'),
    (BLToken.MINUS, r'-'),
    (BLToken.STAR, r'\*'),
    (BLToken.SLASH, r'/'),
    (BLToken.LESS, r'<'),
    (BLToken.GREATER, r'>'),
    (BLToken.ASSIGN, r'='),
    (BLToken.LPAREN, r'\('),
    (BLToken.RPAREN, r'\)'),
    (BLToken.LBRACK, r'\['),
    (BLToken.RBRACK, r'\]'),
    (BLToken.LBRACE, r'\{'),
    (BLToken.RBRACE, r'\}'),
    (BLToken.COMMA, r','),
    (BLToken.COLON, r':'),
    (BLToken.NEWLINE, r'\n'),
    (BLToken.WHITESPACE, r'[ \t]+')
]

_token_re = re.compile('|'.join(f'(?P<{token.name()}>{pattern})' for token, pattern in _token_patterns))
_token_map = {token.name(): token for token, _ in _token_patterns}

class Tokenizer():
    def __init__(self, code: str):
        self.code = code
    
    def count_indent(self, indent: str):
        return len(indent) + indent.count('\t') * (SPACES_IN_TAB - 1)

    def tokenize(self):
        previous_was_newline = False
        indent_stack = [0]

        for match in re.finditer(_token_re, self.code):
            token_name = match.lastgroup
            token = _token_map[token_name]
            value = match.group()
            if previous_was_newline and token != BLToken.NEWLINE:
                previous_was_newline = False

                if token != BLToken.WHITESPACE:
                    while len(indent_stack) > 1:
                        indent_stack.pop()
                        yield (BLToken.DEDENT, "")
                    
                    yield (token, value)
                    continue

                indent_count = self.count_indent(value)
                if indent_count > indent_stack[-1]:
                    indent_stack.append(indent_count)
                    yield (BLToken.INDENT, value)
                elif indent_count < indent_stack[-1]:
                    while indent_stack[-1] > indent_count:
                        yield (BLToken.DEDENT, value)
                        indent_stack.pop()
                    if indent_count != indent_stack[-1]:
                        raise SyntaxError("Inconsistent dedent level")
                continue

            previous_was_newline = (token == BLToken.NEWLINE)

            if token == BLToken.WHITESPACE:
                continue
        
            yield (token, value)
        
        while len(indent_stack) > 1:
            indent_stack.pop()
            yield (BLToken.DEDENT, "")

if __name__ == "__main__":
    print(list(Tokenizer("a\n\tb\n\t\tc\n\t\t\td\n\te").tokenize()))