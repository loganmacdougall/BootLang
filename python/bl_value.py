from dataclasses import dataclass, field
from enum import Enum, auto

class BLType(Enum):
    INT = auto()
    FLOAT = auto()
    STRING = auto()
    LIST = auto()
    DICT = auto()
    FUNCTION = auto()
    BUILTIN_FUNCTION = auto()
    OBJECT = auto()
    NONE = auto()

@dataclass
class BLValue:
    type: BLType
    value: any
    readonly: bool = False

    def get_property(self, name: str, env):
        if name in env.environment_builtins[self.type]:
            return env.environment_builtins[self.type][name]
        else:
            raise RuntimeError(f"Can't find property {name} for type {self.type}")
        
    def set_property(self, name: str, value: "BLValue"):
        raise RuntimeError(f"Can't set property of type {self.type}")
        
    def get_index(self, key: "BLValue"):
        if self.type == BLType.DICT:
            return self.value[key.value]
        elif self.type == BLType.LIST:
            if key.type == BLType.INT:
                return self.value[key.value]
            else:
                raise ValueError(f"Can't index list with type {key.type}")
        else:
            raise ValueError(f"Can't index type {self.type}") 
        
    def set_index(self, key: "BLValue", value: "BLValue"):
        if self.readonly:
            raise RuntimeError(f"Cannot modify readonly object")
        
        if self.type == BLType.DICT:
            self.value[key.value] = value
        elif self.type == BLType.LIST:
            if key.type == BLType.INT:
                self.value[key.value] = value
            else:
                raise ValueError(f"Can't index list with type {key.type}")
        else:
            raise ValueError(f"Can't index type {self.type}") 

        
class BLObjectValue(BLValue):
    fields = dict()

    def __init__(self, fields = None, readonly = False):
        super().__init__(BLType.OBJECT, None, readonly)
        if fields:
            self.fields = fields.copy()

    def get_property(self, name: str, env):
        if name in self.fields:
            return self.fields[name]
        
    def set_property(self, name: str, value: "BLValue"):
        if self.readonly:
            raise RuntimeError(f"Cannot modify readonly object")
        
        self.fields[name] = value
        
    def get_index(self, key: "BLValue"):
        raise ValueError(f"Can't index type {self.type}")   