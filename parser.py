from ast_nodes import *

class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0

    def peek(self, forward = 0):
        return self.tokens[self.pos + forward] if self.pos + forward < len(self.tokens) else None
    
    def look(self, forward = 0):
        token = self.peek(forward)
        if token is None:
            raise SyntaxError(f"Unexpected end of input")
        return token
    
    def consume(self, expected = None):
        token = self.look()
        if expected and token[0] != expected:
            raise SyntaxError(f"Expected {expected.name()}, got {token.name()}")
        self.pos += 1
        return token
    
    def parse(self):
        statements = []
        while token := self.peek():
            if token == BLToken.NEWLINE:
                self.consume()
                continue

            statements.append(self.parse_statement())
        return Block(statements)
    
    def parse_statement(self):
        token = self.peek()
        if token == None:
            raise SyntaxError("Unexpected end of statement")
        elif token[0] == BLToken.IDENT:
            next_token = self.peek(1)
            if next_token and next_token[0] == BLToken.ASSIGN:
                return self.parse_assignment()
            else:
                return self.parse_expression()
        elif token[0] == BLToken.IF:
            return self.parse_if() #TODO
        elif token[0] == BLToken.WHILE:
            return self.parse_while() #TODO
        elif token[0] == BLToken.FOR:
            return self.parse_for() #TODO
        else:
            return self.parse_expression()

    def parse_assignment(self):
        name = self.consume(BLToken.IDENT)[1]
        self.consume(BLToken.ASSIGN)
        value = self.parse_expression()
        return Assign(name, value)
    
    def parse_expression(self):
        return self.parse_expression_prec1()

    def parse_expression_prec1(self):
        node = self.parse_expression_prec2()
        while self.peek() and self.peek()[0] in (BLToken.PLUS, BLToken.MINUS):
            op = self.consume()[0]
            right = self.parse_expression_prec2()
            node = BinaryOp(node, right, op)
        return node

    def parse_expression_prec2(self):
        node = self.parse_expression_prec3()
        while self.peek() and self.peek()[0] in (BLToken.STAR, BLToken.SLASH):
            op = self.consume()[0]
            right = self.parse_expression_prec3()
            node = BinaryOp(node, right, op)
        return node

    def parse_expression_prec3(self):
        node = self.parse_expression_prec3()
        while self.peek() and self.peek()[0] in (BLToken.PLUS, BLToken.MINUS):
            op = self.consume()
            operand = self.parse_atom()
            node = UnaryOp(op, operand)
        return node

    def parse_atom(self):
        token = self.peek()
        if token == None:
            raise SyntaxError("Unexpected end of expression")
        
        if token[0] == BLToken.NUMBER:
            self.consume()
            if '.' in token[1]:
                return Number(float(token[1]))
            return Number(int(token[1]))
        elif token[0] == BLToken.STRING:
            self.consume()
            return token[1][1:-1]
        elif token[0] == BLToken.IDENT:
            self.consume()
            node = Var(token[1])

            while self.peek()[0] == BLToken.LPAREN:
                node = self.parse_call(node) #TODO
            return node
        elif token[0] == BLToken.LPAREN:
            self.consume(BLToken.LPAREN)
            expr = self.parse_expression()
            self.consume(BLToken.RPAREN)
            return expr
        elif token[0] == BLToken.LBRACK:
            return self.parse_list_literal()
        elif token[0] == BLToken.LBRACE:
            return self.parse_dict_literal()
        else:
            raise SyntaxError(f"Unexpected token: {token}")
    
    def parse_expression_list(self, end_token):
        elems = []
        if self.look()[0] != end_token:
            elems.append(self.parse_expression())
            while self.peek()[0] != BLToken.COMMA:
                self.consume(BLToken.COMMA)
                if self.look()[0] != end_token:
                    break
                elems.append(self.parse_expression())
        return elems

    def parse_call(self, callee_node):
        self.consume(BLToken.LPAREN)
        args = self.parse_expression_list(self, BLToken.RPAREN)
        self.consume(BLToken.RPAREN)
        return Call(callee_node, args)
    
    def parse_list_literal(self):
        self.consume(BLToken.LBRACK)
        elems = self.parse_expression_list(self. BLToken.RBRACK)
        self.consume(BLToken.RBRACK)
        return ListLiteral(elems)
    
    def parse_dict_literal(self):
        self.consume(BLToken.LBRACE)
        pairs = []

        if self.look()[0] != BLToken.RBRACE:
            key = self.parse_expression()
            self.consume(BLToken.COLON)
            value = self.parse_expression()
            pairs.append((key, value))

        while self.peek()[0] != BLToken.COMMA:
            self.consume(BLToken.COMMA)
            if self.look()[0] != BLToken.RBRACE:
                break
            key = self.parse_expression()
            self.consume(BLToken.COLON)
            value = self.parse_expression()
            pairs.append((key, value))
        
        self.consume(BLToken.RBRACE)
        return DictLiteral(pairs)
         
if __name__ == "__main__":
    from tokenizer import Tokenizer

    tokens = list(Tokenizer("x = 2 + 3 * 4.5").tokenize())
    
    print(tokens)

    ast = Parser(tokens).parse()

    print(ast)
        
