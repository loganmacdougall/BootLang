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
class SliceRange(BLNode):
    start: BLNode
    end: Optional[BLNode]
    step: Optional[BLNode]

    def to_code(self, _indent = 0):
        return f"[{self.start.to_code() if self.start else ""}:{self.end.to_code() if self.end else ""}:{self.step.to_code() if self.step else ""}]"
    
@dataclass
class Break(BLNode):
    pass

    def to_code(self, _indent = 0):
        return "break"

@dataclass
class Continue(BLNode):
    pass

    def to_code(self, _indent = 0):
        return "continue"
    
@dataclass
class Return(BLNode):
    value: Optional[BLNode]

    def to_code(self, _indent = 0):
        return f"return {self.value.to_code() if self.value else ""}"

@dataclass
class BinaryOp(BLNode):
    left: BLNode
    right: BLNode
    op: BLToken
    
    def to_code(self, _indent = 0):
        return f"({self.left.to_code()} {TOKEN_STRING_MAP[self.op]} {self.right.to_code()})"

@dataclass
class UnaryOp(BLNode):
    op: BLToken
    node: BLNode

    def to_code(self, _indent = 0):
        return f"({TOKEN_STRING_MAP[self.op]}{self.node.to_code()})"

@dataclass
class Assign(BLNode):
    ident: BLNode
    value: BLNode
    op: BLToken

    def to_code(self, _indent = 0):
        return f"{self.ident.to_code()} {TOKEN_STRING_MAP[self.op]} {self.value.to_code()}"

@dataclass
class Ternary(BLNode):
    left: BLNode
    right: BLNode
    cond: BLNode

    def to_code(self, _indent = 0):
        return f"{self.left.to_code()} if {self.cond.to_code()} else {self.right.to_code()}"

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
class Slice(BLNode):
    collection: BLNode
    slice: SliceRange

    def to_code(self, _indent=0):
        return f"{self.collection.to_code()}{self.slice.to_code()}"
    
@dataclass
class Index(BLNode):
    collection: BLNode
    index: BLNode

    def to_code(self, _indent=0):
        return f"{self.collection.to_code()}[{self.index.to_code()}]"

@dataclass
class FunctionDefinition(BLNode):
    name: str
    args: List[str]
    block: Block
    
    def to_code(self, indent = 0):
        return f"def {self.name}({", ".join(self.args)}):\n{self.block.to_code(indent+2)}"

@dataclass
class ListLiteral(BLNode):
    elements: List[BLNode]

    def to_code(self, _indent = 0):
        return f"[{', '.join(elem.to_code() for elem in self.elements)}]"

@dataclass
class DictLiteral(BLNode):
    pairs: List[tuple]

    def to_code(self, _indent = 0):
        return f"{{{', '.join(f"{key.to_code()}:{value.to_code()}" for key, value in self.pairs)}}}"

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
    vars: List[str]
    iterable: BLNode
    block: Block

    def to_code(self, indent = 0):
        return f"for {", ".join(self.vars)} in {self.iterable.to_code()}:\n{self.block.to_code(indent+2)}"
