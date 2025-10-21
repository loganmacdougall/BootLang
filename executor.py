from debugger import Debugger
from environment import Environment
from runtime import Runtime

class Executor:
    def __init__(self):
        self.runtime = Runtime()
        self.debugger = Debugger(self)
        self.globals = Environment(runtime=self.runtime)
        
        self.globals.set("print", print)
        self.globals.set("len", len)
        self.globals.set("range", range)

    def execute(self, node, env=None, debug=False):
        if env is None:
            env = self.globals

        env.debug = debug
        env.debugger = self.debugger

        return node.exec(env)

if __name__ == "__main__":
    import time
    from tokenizer import Tokenizer
    from parser import Parser

    with open("samples/example13.bl", "r") as f:
        code = f.read()

    tokens = list(Tokenizer(code).tokenize())
    ast = Parser(tokens).parse()

    executor = Executor()
    executor.debugger.add_breakpoint(1)

    start = time.time()
    executor.execute(ast, debug=True)
    end = time.time()

    print("--------------")
    print(f"Execution time: {end - start:.3f}")
