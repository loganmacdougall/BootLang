from dataclasses import dataclass
from typing import List, Optional, Union
from tokens import BLToken

@dataclass
class BLNode:
    pass

@dataclass
class Number(BLNode):
    value: int | float

@dataclass
class String(BLNode):
    value: str

@dataclass
class Var(BLNode):
    name: str

@dataclass
class BinaryOp(BLNode):
    left: BLNode
    right: BLNode
    op: BLToken

@dataclass
class UnaryOp(BLNode):
    op: BLToken
    node: BLNode

@dataclass
class Assign(BLNode):
    name: str
    value: BLNode

@dataclass
class Call(BLNode):
    callee: BLNode
    args: List[BLNode]

@dataclass
class ListLiteral(BLNode):
    elements: List[BLNode]

@dataclass
class DictLiteral(BLNode):
    pairs: List[tuple]


@dataclass
class If(BLNode):
    condition: BLNode
    if_block: List[BLNode]
    else_block: Optional[List[BLNode]]

@dataclass
class While(BLNode):
    condition: BLNode
    block: List[BLNode]

@dataclass
class For(BLNode):
    vars: List[str]
    iterable: BLNode
    block: List[BLNode]

@dataclass
class Block(BLNode):
    statements: List[BLNode]
