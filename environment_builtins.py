from bl_value import BLType, BLValue

class EnvironmentBuiltins:
    global_vars = {}
    property_vars = {t:{} for t in BLType}

    def __init__(self):
        pass

    @staticmethod
    def default():
        env_builtins = EnvironmentBuiltins()

        env_builtins.set_global("print", print, BLType.BUILTIN_FUNCTION)
        env_builtins.set_global("len", len, BLType.BUILTIN_FUNCTION)
        env_builtins.set_global("range", range, BLType.BUILTIN_FUNCTION)

        return env_builtins

    def set_global(self, name: str, value: any, value_type: BLType, readonly = True):        
        if self.exists_global(name) and self.global_vars[name].readonly:
            raise RuntimeError(f"Cannot modify readonly object")
        
        wrapped_value = BLValue(value_type, value, readonly)
        self.global_vars[name] = wrapped_value

    def exists_global(self, name: str):
        return name in self.global_vars
    
    def get_global(self, name: str):
        return self.global_vars[name]
    
    def set_property(self, name:str, value: any, type: BLType, value_type: BLType, readonly = True):
        if self.exists_property(name, type) and self.property_vars[type][name].readonly:
            raise RuntimeError(f"Cannot modify readonly object")
        
        wrapped_value = BLValue(value_type, value, readonly)
        self.property_vars[type][name] = wrapped_value

    def exists_property(self, name:str, type: BLType):
        return name in self.property_vars[type]
    
    def get_property(self, name:str, type: BLType):
        return self.property_vars[type][name]
            