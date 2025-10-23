from environment import Environment
from tokenizer import Tokenizer
from parser import Parser

class Executor:
    def __init__(self):        
        pass
    
    def setup_environment(self, env: Environment | None):
        if env == None:
            env = Environment.default()

        env.set("print", print)
        env.set("len", len)
        env.set("range", range)
        env.execute_callback = self.execute_code_without_setup

        return env
    
    def execute_code(self, node, env: Environment | None = None, debug=False):
        env = self.setup_environment(env)
        return self.execute_code_without_setup(node, env, debug)

    def execute(self, node, env: Environment | None = None, debug=False):
        env = self.setup_environment(env)
        return self.execute_without_setup(node, env, debug)

    def execute_code_without_setup(self, code, env: Environment, debug = False):
        tokens = list(Tokenizer(code).tokenize())
        ast = Parser(tokens).parse()

        return self.execute_without_setup(ast, env, debug=debug)
    
    def execute_without_setup(self, node, env: Environment, debug = False):
        previous_debug = env.debugging()

        env.enable_debugging(debug)
        res = node.exec(env)
        env.enable_debugging(previous_debug)

        return res

if __name__ == "__main__":
    import time

    with open("samples/example13.bl", "r") as f:
        code = f.read()

    executor = Executor()
    env = Environment.default()
    # env.debugger.add_breakpoint(1)

    start = time.time()
    executor.execute_code(code, env, debug=True)
    end = time.time()

    print("--------------")
    print(f"Execution time: {end - start:.3f}")
