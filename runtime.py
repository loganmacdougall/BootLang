from typing import List


class CallFrame:
    def __init__(self, func_name, env, node=None):
        self.func_name = func_name
        self.env = env
        self.node = node
        self.line = None
    
class Runtime:
    def __init__(self):
        self.call_stack: List[CallFrame] = []
        self.breakpoints = set()
        
        self.on_enter_frame = None
        self.on_exit_frame = None
        self.on_line = None

    def push_frame(self, frame: CallFrame):
        self.call_stack.append(frame)
        if self.on_enter_frame:
            self.on_enter_frame(self, frame)

    def pop_frame(self):
        frame = self.call_stack.pop()
        if self.on_exit_frame:
            self.on_exit_frame(self, frame)
        return frame
    
    def current_frame(self):
        return self.call_stack[-1] if self.call_stack else None