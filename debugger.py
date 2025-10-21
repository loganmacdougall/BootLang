from enum import Enum
from tokenizer import Tokenizer
from parser import Parser

class DebugMode(Enum):
  CONTINUE = 1,
  STEP_IN = 2,
  STEP_OVER = 3,
  STEP_OUT = 4,
  STOP = 5

class Debugger:
  def __init__(self, executor):
    self.executor = executor
    self.breakpoints = set()
    self.mode = DebugMode.CONTINUE
    self.paused = False
    self.step_depth = 0
    self.call_depth = 0
    self.last_command = ""

  def check(self, node, env):
    if node.lineno in self.breakpoints or self.mode == DebugMode.STEP_IN:
      self.pause(node, env)
      return

    if self.mode == DebugMode.STEP_OVER and self.call_depth <= self.step_depth:
        self.pause(node, env)
        return

    if self.mode == DebugMode.STEP_OUT and self.call_depth < self.step_depth:
        self.pause(node, env)

  def add_breakpoint(self, lineno: int):
    if lineno in self.breakpoints:
      return False
    self.breakpoints.add(lineno)
    return True

  def remove_breakpoint(self, lineno: int):
    if lineno not in self.breakpoints:
      return False
    self.breakpoints.remove(lineno)
    return True

  def pause(self, node, env):
    print(f"Paused at {node.lineno}")
    self.paused = True

    self.interactive_cli(node, env)
  
  def interactive_cli(self, node, env):
    while self.paused:
      print(node.to_code().split("\n")[0])
      cmd = input("(debug) ").strip()
      if cmd == "":
        cmd = self.last_command
      self.last_command = cmd

      if cmd == "c":
        self.mode = DebugMode.CONTINUE
        self.paused = False
      elif cmd == "i":
        self.mode = DebugMode.STEP_IN
        self.paused = False
      elif cmd == "s":
        self.mode = DebugMode.STEP_OVER
        self.step_depth = self.call_depth
        self.paused = False
      elif cmd == "o":
        self.mode = DebugMode.STEP_OUT
        self.step_depth = self.call_depth - 1
        self.paused = False
      elif cmd.startswith("b "):
        parts = cmd.split()
        if len(parts) != 2 or not parts[1].isdigit():
          print("Usage: b <line>")
          continue

        line = int(parts[1])
        if self.add_breakpoint(line):
          print(f"Breakpoint set at line {line}")
        else:
          print(f"Breakpoint already at line {line}")
      elif cmd.startswith("db "):
        parts = cmd.split()
        if len(parts) != 2 or not parts[1].isdigit():
          print("Usage: db <line>")
          continue

        line = int(parts[1])
        if self.remove_breakpoint(line):
          print(f"Breakpoint removed at line {line}")
        else:
          print(f"No breakpoint at line {line}")
      elif cmd == "q":
        self.mode = DebugMode.STOP
        print("Execution stopped")
        raise SystemExit
      elif cmd.startswith("p "):
        watch_str = cmd[2:]
        watch_tokens = list(Tokenizer(watch_str).tokenize())
        watch_ast = Parser(watch_tokens).parse()
        env.debug = False
        result = self.executor.execute(watch_ast, env, False)
        env.debug = True

        print(result)


          



