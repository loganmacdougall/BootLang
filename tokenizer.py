import re
from tokens import BLToken

_token_patterns = [
    (BLToken.NUMBER, r'\d+(\.\d+)?'),
    (BLToken.STRING, r'"[^"]*"'),
    (BLToken.IDENT, r'[A-Za-z_][A-Za-z_0-9]*'),
    (BLToken.PLUS, r'\+'),
    (BLToken.MINUS, r'-'),
    (BLToken.STAR, r'\*'),
    (BLToken.SLASH, r'/'),
    (BLToken.EQUAL, r'=='),
    (BLToken.NEQUAL, r'!='),
    (BLToken.LESS_EQUAL, r'<='),
    (BLToken.GREATER_EQUAL, r'>='),
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
    (BLToken.SKIP, r'[ \t]+')
]

_token_re = re.compile('|'.join(f'(?P<{token.name()}>{pattern})' for token, pattern in _token_patterns))
_token_map = {token.name(): token for token, _ in _token_patterns}

class Tokenizer():
    def __init__(self, code: str):
        self.code = code

    def tokenize(self):
        for match in re.finditer(_token_re, self.code):
            token_name = match.lastgroup
            token = _token_map[token_name]
            value = match.group()
            if token == BLToken.SKIP:
                continue
            yield (token, value)

if __name__ == "__main__":
    with open("samples/example01.bl", "r") as f:
        code = f.read()

    print(list(Tokenizer(code).tokenize()))