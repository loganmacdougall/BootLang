from enum import Enum

class ControlSignal(Enum):
    NONE = 1
    BREAK = 2
    CONTINUE = 3
    RETURN = 4

class Environment:
    def __init__(self, parent=None, runtime=None, debugger=None, debug=False):
        self.vars = {}
        self.parent = parent
        self.runtime = runtime or (parent.runtime if parent else None)
        self.debugger = debugger or (parent.debugger if parent else None)
        self.debug = debug or (parent.debug if parent else False)
        self.return_value = None
        self.signal = ControlSignal.NONE

    def peek(self, name):
        if name in self.vars:
            return self.vars[name]
        if self.parent:
            return self.parent.peek(name)
        return False

    def get(self, name):
        if name in self.vars:
            return self.vars[name]
        if self.parent:
            return self.parent.get(name)
        raise NameError(f"Undefined variable '{name}'")
    
    def set(self, name, value):
        self.vars[name] = value

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
        else:
            raise NameError(f"Undefined variable '{name}'")