from dataclasses import dataclass
from typing import List, Optional, Union
from tokens import BLToken, TOKEN_STRING_MAP

@dataclass
class BLNode:
    pass

@dataclass
class Number(BLNode):
    value: int | float

    def to_code(self, _indent = 0):
        return f"{self.value}"

@dataclass
class String(BLNode):
    value: str

    def to_code(self, _indent = 0):
        return f"\"{self.value}\""

@dataclass
class Var(BLNode):
    name: str
    
    def to_code(self, _indent = 0):
        return f"{self.name}"

@dataclass
class BinaryOp(BLNode):
    left: BLNode
    right: BLNode
    op: BLToken
    
    def to_code(self, _indent = 0):
        return f"{self.left.to_code()} {TOKEN_STRING_MAP[self.op]} {self.right.to_code()}"

@dataclass
class UnaryOp(BLNode):
    op: BLToken
    node: BLNode

    def to_code(self, _indent = 0):
        return f"{TOKEN_STRING_MAP[self.op]}{self.node.to_code()}"

@dataclass
class Assign(BLNode):
    name: str
    value: BLNode
    op: BLToken

    def to_code(self, _indent = 0):
        return f"{self.name} {TOKEN_STRING_MAP[self.op]} {self.value.to_code()}"

@dataclass
class Block(BLNode):
    statements: List[BLNode]

    def to_code(self, indent = 0):
        i = " " * indent
        return i + f"\n{i}".join(s.to_code(indent) for s in self.statements)

@dataclass
class Call(BLNode):
    callee: BLNode
    args: List[BLNode]

    def to_code(self, _indent = 0):
        return f"{self.callee.to_code()}({", ".join(arg.to_code() for arg in self.args)})"

@dataclass
class ListLiteral(BLNode):
    elements: List[BLNode]

    def to_code(self, _indent = 0):
        return f"[{', '.join(elem.to_code() for elem in self.elements)}]"

@dataclass
class DictLiteral(BLNode):
    pairs: List[tuple]

    def to_code(self, _indent = 0):
        return f"[{', '.join(f"{key.to_code()}:{value.to_code()}" for key, value in self.pairs)}]"

@dataclass
class If(BLNode):
    condition: BLNode
    if_block: Block
    elif_blocks: List[tuple[BLNode, Block]]
    else_block: Optional[Block]

    def to_code(self, indent = 0):
        if_line = "if {}:"
        elif_line = (" " * indent) + "elif {}:"
        else_line = (" " * indent) + "else:"

        lines = []

        lines.append(if_line.format(self.condition.to_code()))
        lines.append(self.if_block.to_code(indent + 2))
        
        for cond, block in self.elif_blocks:
            lines.append(elif_line.format(cond.to_code()))
            lines.append(block.to_code(indent + 2))

        if self.else_block:
            lines.append(else_line)
            lines.append(self.else_block.to_code(indent + 2))

        return "\n".join(lines)

@dataclass
class While(BLNode):
    condition: BLNode
    block: Block

    def to_code(self, indent = 0):
        return f"while {self.condition.to_code()}:\n{self.block.to_code(indent+2)}"

@dataclass
class For(BLNode):
    vars: List[Var]
    iterable: BLNode
    block: Block

    def to_code(self, indent = 0):
        return f"for {", ".join(v.to_code() for v in self.vars)} in {self.iterable.to_code()}:\n{self.block.to_code(indent+2)}"
