from environment import Environment, ControlSignal
from runtime import CallFrame, Runtime


class FunctionValue:
    def __init__(self, name, params, body, closure_env, meta=None):
        self.name = name
        self.params = list(params)
        self.body = body
        self.closure_env = closure_env
        self.meta = meta or {}

    def call(self, args, runtime: Runtime | None = None):
        env = Environment(self.closure_env, runtime)

        for i, name in enumerate(self.params):
            env.set(name, args[i] if i < len(args) else None)

        if runtime:
            frame = CallFrame(self.name, env, node=self.body)
            runtime.push_frame(frame)

        self.body.exec(env)

        ret = None
        if env.get_signal() == ControlSignal.RETURN:
            ret = env.get_return()
            env.set_signal(ControlSignal.NONE)

        if runtime:
            runtime.pop_frame()

        return ret