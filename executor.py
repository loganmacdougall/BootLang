from environment import Environment
from runtime import Runtime

class Executor:
    def __init__(self):
        self.runtime = Runtime()
        self.globals = Environment(runtime=self.runtime)
        
        self.globals.set("print", print)
        self.globals.set("len", len)
        self.globals.set("range", range)

    def execute(self, node, env=None):
        if env is None:
            env = self.globals

        node.exec(env)

if __name__ == "__main__":
    from tokenizer import Tokenizer
    from parser import Parser

    with open("samples/example12.bl", "r") as f:
        code = f.read()

    tokens = list(Tokenizer(code).tokenize())
    ast = Parser(tokens).parse()
    Executor().execute(ast)
