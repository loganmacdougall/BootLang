from ast_nodes import *
from tokenizer import TokenData

WHITESPACE_TOKENS = (BLToken.NEWLINE, BLToken.INDENT, BLToken.DEDENT)
UNESCAPE_MAP = {
    "'": "'", "\\": "\\", "n": "\n", "r": "\r", "t": "\t", "b": "\b", "f": "\f"
}

class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0
        self.ignore_whitespace = False

    def peek(self, forward = 0) -> TokenData | None:
        if self.ignore_whitespace:
            self.consume_whitespace()

        token = self.tokens[self.pos + forward] if self.pos + forward < len(self.tokens) else None
        while self.ignore_whitespace and token and token.token in WHITESPACE_TOKENS:
            forward += 1
            token = self.tokens[self.pos + forward] if self.pos + forward < len(self.tokens) else None
        
        return token
    
    def look(self, forward = 0) -> TokenData:
        token = self.peek(forward)
        if token is None:
            raise SyntaxError(f"Unexpected end of input")
        return token
    
    def ignoreWhitespace(self, value: bool):
        self.ignore_whitespace = value

    def consume(self, expected = None) -> TokenData:
        token = self.look()
        if expected and token.token != expected:
            raise SyntaxError(f"Expected {expected.name()}, got {token.token.name()}")
        self.pos += 1
        return token
    
    def consume_whitespace(self):
        token = self.tokens[self.pos] if self.pos < len(self.tokens) else None

        while token and token.token in WHITESPACE_TOKENS:
            self.pos += 1
            token = self.tokens[self.pos] if self.pos < len(self.tokens) else None
    
    def parse(self):
        statements = []
        while token := self.peek():
            if token.token == BLToken.NEWLINE:
                self.consume()
                continue

            statements.append(self.parse_statement())
        return Block(1, 1, statements)
    
    def parse_statement(self):
        token = self.peek()
        if token == None:
            raise SyntaxError("Unexpected end of statement")
        elif token.token == BLToken.IDENT:
            forward = 1
            while token := self.peek(forward):
                if token.token in (
                BLToken.ASSIGN, BLToken.PLUS_ASSIGN, BLToken.MINUS_ASSIGN,
                BLToken.STAR_ASSIGN, BLToken.SLASH_ASSIGN, BLToken.PERCENT_ASSIGN
                    ):
                    return self.parse_assignment()
                if token.token == BLToken.NEWLINE:
                    break
                forward += 1
            return self.parse_expression()
        elif token.token == BLToken.BREAK:
            self.consume(BLToken.BREAK)
            return Break()
        elif token.token == BLToken.CONTINUE:
            self.consume(BLToken.CONTINUE)
            return Continue()
        elif token.token == BLToken.IF:
            return self.parse_if()
        elif token.token == BLToken.WHILE:
            return self.parse_while()
        elif token.token == BLToken.FOR:
            return self.parse_for()
        elif token.token == BLToken.DEF:
            return self.parse_function_definition()
        elif token.token == BLToken.RETURN:
            return self.parse_return()
        else:
            return self.parse_expression()

    def parse_ident(self):
        token = self.consume(BLToken.IDENT)
        node = Var(token.lineno, token.col, token.string)

        while True:
            if token := self.peek():
                if token.token == BLToken.LPAREN:
                    node = self.parse_call(node)
                    continue
                if token.token == BLToken.LBRACK:
                    node = self.parse_index_or_slice(node)
                    continue
                if token.token == BLToken.DOT:
                    node = self.parse_property_access(node)
                    continue
            break
        return node

    def parse_assignment(self):
        ident = self.parse_ident()
        op = self.consume().token
        value = self.parse_expression()
        return Assign(ident.lineno, ident.col, ident, value, op)

    def parse_slice(self):
        start_node = self.consume(BLToken.LBRACK)
        
        start = None
        end = None
        step = None

        if self.look().token != BLToken.COLON:
            start = self.parse_expression()

        if self.look().token != BLToken.RBRACK:
            self.consume(BLToken.COLON)

            if self.look().token not in (BLToken.RBRACK, BLToken.COLON):
                end = self.parse_expression()

            if self.look().token == BLToken.COLON:
                self.consume(BLToken.COLON)
                
                if self.look().token != BLToken.RBRACK:
                    step = self.parse_expression()

        self.consume(BLToken.RBRACK)
        return SliceRange(start_node.lineno, start_node.col, start, end, step)

    def parse_expression(self):
        return self.parse_ternary()
    
    def parse_ternary(self):
        node = self.parse_boolean_ops()
        while self.peek() and self.look().token == BLToken.IF:
            self.consume(BLToken.IF)
            cond = self.parse_expression()
            self.consume(BLToken.ELSE)
            right = self.parse_expression()
            node = Ternary(node.lineno, node.col, node, right, cond)
        return node
    
    def parse_boolean_ops(self):
        node = self.parse_boolean_not()
        while self.peek() and self.look().token in (BLToken.AND, BLToken.OR):
            op = self.consume().token
            right = self.parse_boolean_not()
            node = BinaryOp(node.lineno, node.col, node, right, op)
        return node
    
    def parse_boolean_not(self):
        while self.peek() and self.look().token == BLToken.NOT:
            unary_node = self.consume(BLToken.NOT)
            right = self.parse_comparison()
            return UnaryOp(unary_node.lineno, unary_node.col, BLToken.NOT, right)
        return self.parse_comparison()

    def parse_comparison(self):
        node = self.parse_additive()
        while self.peek() and self.look().token in (
            BLToken.LESS, BLToken.GREATER, BLToken.LESS_EQUAL,
            BLToken.GREATER_EQUAL, BLToken.EQUAL, BLToken.NEQUAL
        ):
            op = self.consume().token
            right = self.parse_additive()
            node = BinaryOp(node.lineno, node.col, node, right, op)
        return node

    def parse_additive(self):
        node = self.parse_multiplicative()
        while self.peek() and self.look().token in (BLToken.PLUS, BLToken.MINUS, BLToken.PERCENT):
            op = self.consume().token
            right = self.parse_multiplicative()
            node = BinaryOp(node.lineno, node.col, node, right, op)
        return node

    def parse_multiplicative(self):
        node = self.parse_unary()
        while self.peek() and self.look().token in (BLToken.STAR, BLToken.SLASH):
            op = self.consume().token
            right = self.parse_unary()
            node = BinaryOp(node.lineno, node.col, node, right, op)
        return node

    def parse_unary(self):
        if self.peek() and self.look().token in (BLToken.PLUS, BLToken.MINUS):
            op = self.consume()
            operand = self.parse_unary()
            return UnaryOp(op.lineno, op.col, op.token, operand)
        return self.parse_atom()

    def parse_atom(self):
        token = self.peek()
        if token == None:
            raise SyntaxError("Unexpected end of expression")
        
        if token.token == BLToken.NUMBER:
            self.consume()
            if '.' in token.string:
                return Number(token.lineno, token.col, float(token.string))
            return Number(token.lineno, token.col, int(token.string))
        elif token.token == BLToken.STRING:
            self.consume()
            unescaped_string = self.unescape_string(token.string[1:-1])
            return String(token.lineno, token.col, unescaped_string)
        elif token.token in (BLToken.TRUE, BLToken.FALSE):
            self.consume()
            return Bool(token.lineno, token.col, True if BLToken.TRUE else False)
        elif token.token == BLToken.IDENT:
            return self.parse_ident()
        elif token.token == BLToken.LPAREN:
            self.consume(BLToken.LPAREN)
            expr = self.parse_expression()
            self.consume(BLToken.RPAREN)
            return expr
        elif token.token == BLToken.LBRACK:
            return self.parse_list_literal()
        elif token.token == BLToken.LBRACE:
            return self.parse_dict_literal()
        else:
            raise SyntaxError(f"Unexpected token: {token}")
        
    def unescape_string(self, s: str):
        chars = []
        unescape = False

        i = 0
        while i < len(s):
            c = s[i]
            if c != "\\" and not unescape:
                chars.append(c)
            elif c == "\\" and not unescape:
                unescape = True
            elif unescape and c in UNESCAPE_MAP:
                chars.append(UNESCAPE_MAP[c])
                unescape = False
            elif unescape and c == "x":
                hex_str = s[i+1:i+3]
                chars.append(chr(int(hex_str, 16)))
                i += 2
            elif unescape and ord("0") <= ord(c) <= ord("9"):
                octal_str = s[i+1:i+4]
                chars.append(chr(int(octal_str, 8)))
            i += 1
        
        return "".join(chars)
    
    def parse_expression_list(self, end_token):
        self.ignoreWhitespace(True)
        elems = []
        if self.look().token != end_token:
            elems.append(self.parse_expression())
            while self.look().token == BLToken.COMMA:
                self.consume(BLToken.COMMA)
                if self.look().token == end_token:
                    break
                elems.append(self.parse_expression())
        self.ignoreWhitespace(False)
        return elems
    
    def parse_ident_list(self):
        vars = []

        token = self.peek()
        
        if not token or token.token != BLToken.IDENT:
            return vars

        token = self.consume(BLToken.IDENT)
        vars.append(token.string)
        
        while self.peek() and self.look().token == BLToken.COMMA:
            self.consume(BLToken.COMMA)
            token = self.consume(BLToken.IDENT)
            vars.append(token.string)

        return vars

    def parse_property_access(self, callee_node):
        self.consume(BLToken.DOT)
        name = self.consume(BLToken.IDENT)[1]
        return PropertyAccess(callee_node.lineno, callee_node.col, callee_node, name)

    def parse_call(self, callee_node):
        self.consume(BLToken.LPAREN)
        args = self.parse_expression_list(BLToken.RPAREN)
        self.consume(BLToken.RPAREN)
        return Call(callee_node.lineno, callee_node.col, callee_node, args)
    
    def parse_index_or_slice(self, collection):
        slice = self.parse_slice()
        if slice.start and not slice.end and not slice.step:
            return Index(collection.lineno, collection.col, collection, slice.start)
        else:
            return Slice(collection.lineno, collection.col, collection, slice)
    
    def parse_function_definition(self):
        start_node = self.consume(BLToken.DEF)
        name = self.consume(BLToken.IDENT).string
        self.consume(BLToken.LPAREN)
        args = self.parse_ident_list()
        self.consume(BLToken.RPAREN)
        self.consume(BLToken.COLON)
        block = self.parse_block()

        return FunctionDefinition(start_node.lineno, start_node.col, name, args, block)
    
    def parse_list_literal(self):
        start_node = self.consume(BLToken.LBRACK)
        self.ignoreWhitespace(True)
        elems = self.parse_expression_list(BLToken.RBRACK)
        self.ignoreWhitespace(False)
        self.consume(BLToken.RBRACK)
        return ListLiteral(start_node.lineno, start_node.col, elems)
    
    def parse_dict_literal(self):
        start_node = self.consume(BLToken.LBRACE)
        self.ignoreWhitespace(True)
        pairs = []

        if self.look().token != BLToken.RBRACE:
            key = self.parse_expression()
            self.consume(BLToken.COLON)
            value = self.parse_expression()
            pairs.append((key, value))

        while self.look().token == BLToken.COMMA:
            self.consume(BLToken.COMMA)
            if self.look().token == BLToken.RBRACE:
                break
            key = self.parse_expression()
            self.consume(BLToken.COLON)
            value = self.parse_expression()
            pairs.append((key, value))
        
        self.ignoreWhitespace(False)
        self.consume(BLToken.RBRACE)
        return DictLiteral(start_node.lineno, start_node.col, pairs)
    
    def parse_block(self):
        self.consume(BLToken.NEWLINE)
        start_node = self.consume(BLToken.INDENT)

        statements = []
        while (token := self.look()).token != BLToken.DEDENT:
            if token.token == BLToken.NEWLINE:
                self.consume()
                continue
            statements.append(self.parse_statement())
        self.consume(BLToken.DEDENT)
        return Block(start_node.lineno, start_node.col, statements)
    
    def parse_return(self):
        start_node = self.consume(BLToken.RETURN)

        if self.look().token == BLToken.NEWLINE:
            return Return(start_node.lineno, start_node.col, None)

        value = self.parse_expression()

        return Return(start_node.lineno, start_node.col, value)

    def parse_if(self):
        start_node = self.consume(BLToken.IF)
        condition = self.parse_expression()
        self.consume(BLToken.COLON)
        if_block = self.parse_block()

        elif_blocks = []
        else_block = None

        while self.peek() and self.look().token == BLToken.ELIF:
            self.consume(BLToken.ELIF)
            elif_cond = self.parse_expression()
            self.consume(BLToken.COLON)
            elif_block = self.parse_block()
            elif_blocks.append((elif_cond, elif_block))

        if self.peek() and self.look().token == BLToken.ELSE:
            self.consume(BLToken.ELSE)
            self.consume(BLToken.COLON)
            else_block = self.parse_block()

        return If(start_node.lineno, start_node.col, condition, if_block, elif_blocks, else_block)
    
    def parse_while(self):
        start_node = self.consume(BLToken.WHILE)
        condition = self.parse_expression()
        self.consume(BLToken.COLON)
        block = self.parse_block()

        return While(start_node.lineno, start_node.col, condition, block)
    
    def parse_for(self):
        start_node = self.consume(BLToken.FOR)
        parentheses = False

        if self.look().token == BLToken.LPAREN:
            self.consume(BLToken.RPAREN)
            parentheses = True

        vars = self.parse_ident_list()

        if parentheses:
            self.consume(BLToken.RPAREN)

        if len(vars) == 0:
            raise SyntaxError("You can't have 0 arguments in a for loop. Use '_' if you don't want to use the result from the iterable.")
        
        self.consume(BLToken.IN)
        iterable = self.parse_expression()
        self.consume(BLToken.COLON)
        block = self.parse_block()

        return For(start_node.lineno, start_node.col, vars, iterable, block)
         
if __name__ == "__main__":
    from tokenizer import Tokenizer

    with open("samples/example11.bl", "r") as f:
        code = f.read()

    tokens = list(Tokenizer(code).tokenize())
    ast = Parser(tokens).parse()
    print(ast.to_code())
        
