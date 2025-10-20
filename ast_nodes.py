from environment import Environment, ControlSignal
from dataclasses import dataclass
from typing import List, Optional
from function_value import FunctionValue
from tokens import BLToken, TOKEN_STRING_MAP

@dataclass
class BLNode:
    pass

    def to_code(self, _indent = 0):
        return ""
    
    def exec(self, _env: Environment):
        pass

@dataclass
class Number(BLNode):
    value: int | float

    def to_code(self, _indent = 0):
        return f"{self.value}"
    
    def exec(self, _env: Environment):
        return self.value

@dataclass
class String(BLNode):
    value: str

    def to_code(self, _indent = 0):
        return f"\"{self.value}\""
    
    def exec(self, _env: Environment):
        return self.value
    
@dataclass
class Bool(BLNode):
    value: bool

    def to_code(self, _indent = 0):
        return "True" if self.value else "False"
    
    def exec(self, _env: Environment):
        return self.value

@dataclass
class Var(BLNode):
    name: str
    
    def to_code(self, _indent = 0):
        return f"{self.name}"
    
    def resolve_setter(self, env: Environment):
        def setter(x):
            env.set(self.name, x)
        return (setter, env.peek(self.name))
    
    def exec(self, env: Environment):
        return env.get(self.name)
    
@dataclass
class SliceRange(BLNode):
    start: Optional[BLNode]
    end: Optional[BLNode]
    step: Optional[BLNode]

    def to_code(self, _indent = 0):
        return f"[{self.start.to_code() if self.start else ""}:{self.end.to_code() if self.end else ""}:{self.step.to_code() if self.step else ""}]"
    
@dataclass
class Break(BLNode):
    pass

    def to_code(self, _indent = 0):
        return "break"
    
    def exec(self, env: Environment):
        env.set_signal(ControlSignal.BREAK)

@dataclass
class Continue(BLNode):
    pass

    def to_code(self, _indent = 0):
        return "continue"
    
    def exec(self, env: Environment):
        env.set_signal(ControlSignal.CONTINUE)
    
@dataclass
class Return(BLNode):
    value: Optional[BLNode]

    def to_code(self, _indent = 0):
        return f"return {self.value.to_code() if self.value else ""}"
    
    def exec(self, env: Environment):
        value = self.value.exec(env) if self.value else None
        env.set_signal(ControlSignal.RETURN)
        env.set_return(value)

@dataclass
class BinaryOp(BLNode):
    left: BLNode
    right: BLNode
    op: BLToken
    
    def to_code(self, _indent = 0):
        return f"({self.left.to_code()} {TOKEN_STRING_MAP[self.op]} {self.right.to_code()})"
    
    def exec(self, env: Environment):
        left_value = self.left.exec(env)
        right_value = self.right.exec(env)
        return BINARY_OP_FUNC_MAP[self.op](left_value, right_value)

@dataclass
class UnaryOp(BLNode):
    op: BLToken
    node: BLNode

    def to_code(self, _indent = 0):
        return f"({TOKEN_STRING_MAP[self.op]}{self.node.to_code()})"

    def exec(self, env: Environment):
        value = self.node.exec(env)
        return UNARY_OP_FUNC_MAP[self.op](value)

@dataclass
class Assign(BLNode):
    ident: BLNode
    value: BLNode
    op: BLToken

    def to_code(self, _indent = 0):
        return f"{self.ident.to_code()} {TOKEN_STRING_MAP[self.op]} {self.value.to_code()}"
    
    def exec(self, env: Environment):
        if not hasattr(self.ident, "resolve_setter"):
            raise SyntaxError("Attempted to assign value to non-identifier")
        
        ident_setter, ident_value = self.ident.resolve_setter(env)
        value = self.value.exec(env)

        if ident_value != None:
            ident_setter(ASSIGN_OP_FUCN_MAP[self.op](ident_value, value)) # FIX Can't use ident as name and value
        elif self.op != BLToken.ASSIGN:
            raise SyntaxError("Attempted to use variable before declaring it")
        else:
            ident_setter(value)


@dataclass
class Ternary(BLNode):
    left: BLNode
    right: BLNode
    cond: BLNode

    def to_code(self, _indent = 0):
        return f"{self.left.to_code()} if {self.cond.to_code()} else {self.right.to_code()}"
    
    def exec(self, env: Environment):
        return self.left.exec(env) if self.cond.exec(env) else self.right.exec(env)

@dataclass
class Block(BLNode):
    statements: List[BLNode]

    def to_code(self, indent = 0):
        i = " " * indent
        return i + f"\n{i}".join(s.to_code(indent) for s in self.statements)
    
    def exec(self, env: Environment):
        result = None
        for stmt in self.statements:
            result = stmt.exec(env)
            if env.get_signal() != ControlSignal.NONE:
                break
        return result

            

@dataclass
class Call(BLNode):
    callee: BLNode
    args: List[BLNode]

    def to_code(self, _indent = 0):
        return f"{self.callee.to_code()}({", ".join(arg.to_code() for arg in self.args)})"
    
    def exec(self, env: Environment):
        runtime = env.runtime if env.runtime else None

        func_value = self.callee.exec(env)
        arg_values = [arg.exec(env) for arg in self.args]

        if isinstance(func_value, FunctionValue):
            return func_value.call(arg_values, runtime)
        
        if callable(func_value):
            return func_value(*arg_values)
        
        raise TypeError(f"Object {func_value} is not callable")
        
    
@dataclass
class Slice(BLNode):
    collection: BLNode
    slice: SliceRange

    def to_code(self, _indent=0):
        return f"{self.collection.to_code()}{self.slice.to_code()}"
    
    def exec(self, env: Environment):
        collection_value = self.collection.exec(env)
        start = self.slice.start.exec(env) if self.slice.start else None
        end = self.slice.end.exec(env) if self.slice.end else None
        step = self.slice.step.exec(env) if self.slice.step else None
        return collection_value[slice(start, end, step)]

    
@dataclass
class Index(BLNode):
    collection: BLNode
    index: BLNode

    def to_code(self, _indent=0):
        return f"{self.collection.to_code()}[{self.index.to_code()}]"
    
    def resolve_setter(self, env: Environment):
        collection_value = self.collection.exec(env)
        index_value = self.index.exec(env)
        value = None

        if isinstance(collection_value, dict):
            value = collection_value.get(index_value, None)
        elif isinstance(collection_value, list):
            if 0 <= index_value < len(collection_value):
                value = collection_value[index_value]
        else:
            raise TypeError(f"Cannot index into non-collection type")
        
        def setter(x):
            collection_value[index_value] = x

        return (setter, value)
    
    def exec(self, env: Environment):
        collection_value = self.collection.exec(env)
        index_value = self.index.exec(env)
        return collection_value[index_value]

@dataclass
class FunctionDefinition(BLNode):
    name: str
    args: List[str]
    block: Block
    
    def to_code(self, indent = 0):
        return f"def {self.name}({", ".join(self.args)}):\n{self.block.to_code(indent+2)}"
    
    def exec(self, env: Environment):
        fn = FunctionValue(self.name, self.args, self.block, env, meta={"name": self.name})
        env.set(self.name, fn)
        return fn

@dataclass
class ListLiteral(BLNode):
    elements: List[BLNode]

    def to_code(self, _indent = 0):
        return f"[{', '.join(elem.to_code() for elem in self.elements)}]"
    
    def exec(self, env: Environment):
        return [elem.exec(env) for elem in self.elements]

@dataclass
class DictLiteral(BLNode):
    pairs: List[tuple]

    def to_code(self, _indent = 0):
        return f"{{{', '.join(f"{key.to_code()}:{value.to_code()}" for key, value in self.pairs)}}}"
    
    def exec(self, env: Environment):
        return {k.exec(env):v.exec(env) for k,v in self.pairs}

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
    
    def exec(self, env: Environment):
        if self.condition.exec(env):
            return self.if_block.exec(env)
        
        if self.elif_blocks:
            for cond, block in self.elif_blocks:
                if cond.exec(env):
                    return block.exec(env)
                
        if self.else_block:
            return self.else_block.exec(env)

@dataclass
class While(BLNode):
    condition: BLNode
    block: Block

    def to_code(self, indent = 0):
        return f"while {self.condition.to_code()}:\n{self.block.to_code(indent+2)}"
    
    def exec(self, env: Environment):
        while self.condition.exec(env):
            result = self.block.exec(env)

            if env.get_signal() == ControlSignal.BREAK:
                env.set_signal(ControlSignal.NONE)
                break
            elif env.get_signal() == ControlSignal.RETURN:
                return result

@dataclass
class For(BLNode):
    vars: List[str]
    iterable: BLNode
    block: Block

    def to_code(self, indent = 0):
        return f"for {", ".join(self.vars)} in {self.iterable.to_code()}:\n{self.block.to_code(indent+2)}"
    
    def exec(self, env: Environment):
        items = self.iterable.exec(env)
        if isinstance(items, dict):
            items = items.items()

        for item in items:
            item = list(item)
            for i, value in enumerate(item):
                env.set(self.vars[i], value)

            result = self.block.exec(env)

            signal = env.get_signal()
            if signal == ControlSignal.BREAK:
                env.set_signal(ControlSignal.NONE)
                break
            elif signal == ControlSignal.CONTINUE:
                env.set_signal(ControlSignal.NONE)
                continue
            elif signal == ControlSignal.RETURN:
                return result
    
BINARY_OP_FUNC_MAP = {
    BLToken.PLUS: lambda a,b : a + b,
    BLToken.MINUS: lambda a,b : a - b,
    BLToken.STAR: lambda a,b : a * b,
    BLToken.SLASH: lambda a,b: a / b,
    BLToken.PERCENT: lambda a,b : a % b,
    BLToken.EQUAL: lambda a,b : a == b,
    BLToken.NEQUAL: lambda a,b : a != b,
    BLToken.LESS: lambda a,b : a < b,
    BLToken.GREATER: lambda a,b : a > b,
    BLToken.LESS_EQUAL: lambda a,b : a <= b,
    BLToken.GREATER_EQUAL: lambda a,b: a >= b,
    BLToken.AND : lambda a,b: a and b,
    BLToken.OR : lambda a,b : a or b
}

UNARY_OP_FUNC_MAP = {
    BLToken.PLUS: lambda b: +b,
    BLToken.MINUS: lambda b: -b,
    BLToken.NOT: lambda b: not b
}

ASSIGN_OP_FUCN_MAP = {
    BLToken.ASSIGN: lambda l, r: r,
    BLToken.PLUS_ASSIGN: lambda l, r: l + r,
    BLToken.MINUS_ASSIGN: lambda l, r: l - r,
    BLToken.STAR_ASSIGN: lambda l, r: l * r,
    BLToken.SLASH_ASSIGN: lambda l, r: l / r,
    BLToken.PERCENT_ASSIGN: lambda l, r: l % r
}