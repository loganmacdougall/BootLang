from enum import Enum
from debugger import Debugger
from runtime import Runtime
from environment_builtins import EnvironmentBuiltins

class ControlSignal(Enum):
    NONE = 1
    BREAK = 2
    CONTINUE = 3
    RETURN = 4

class Environment:
    def __init__(self, parent=None, runtime=None, debugger=None, execute_callback=None, environment_builtins=None, debug=False):
        self.vars = {}
        self.parent = parent
        self.runtime = runtime or (parent.runtime if parent else None)
        self.debugger = debugger or (parent.debugger if parent else Debugger())
        self.debug = debug or (parent.debug if parent else False)
        self.execute_callback = execute_callback or (parent.execute_callback if parent else None)
        self.environment_builtins = environment_builtins or (parent.environment_builtins if parent else EnvironmentBuiltins())
        self.return_value = None
        self.signal = ControlSignal.NONE

    @staticmethod
    def default():
        return Environment(runtime=Runtime(), debugger=Debugger(), debug=False)
    
    def execute(self, node):
        if self.execute_callback:
            return self.execute_callback(node, self, False)
        raise ValueError("Execute callback doesn't exist")

    def peek(self, name):
        if name in self.vars:
            return self.vars[name]
        if self.parent:
            return self.parent.peek(name)
        if self.environment_builtins.exists_global(name):
            return self.environment_builtins.get_global(name)
        return False

    def get(self, name):
        if name in self.vars:
            return self.vars[name]
        if self.parent:
            return self.parent.get(name)
        if self.environment_builtins.exists_global(name):
            return self.environment_builtins.get_global(name)
        raise NameError(f"Undefined variable '{name}'")
    
    def set(self, name, value):
        self.vars[name] = value

    def enable_debugging(self, value: bool):
        self.debug = value

    def debugging(self):
        return self.debug

    def get_signal(self):
        return self.signal
    
    def set_signal(self, signal):
        self.signal = signal

    def get_return(self):
        return self.return_value

    def set_return(self, value):
        self.return_value = value

    def update(self, name, value):
        if name in self.vars:
            self.vars[name] = value
        elif self.parent:
            self.parent.update(name, value)
        elif self.environment_builtins.exists_global(name):
            return self.environment_builtins.set_global(name, value, value.type, readonly=False)
        else:
            raise NameError(f"Undefined variable '{name}'")