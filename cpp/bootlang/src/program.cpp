#include "program.hpp"

Program::Program(std::shared_ptr<TopLevelContext> context, std::shared_ptr<std::vector<std::shared_ptr<CodeObject>>> funcs, const Environment& env)
    : context(context), funcs(funcs), env(env) {}

std::string Program::toDisassembly() const {
    std::ostringstream out;

    out << context->toDisassembly() << std::endl;

    for (auto it = funcs->begin(); it != funcs->end(); it++) {
        auto code = *it;
        out << code->toDisassembly();
    }

    return out.str();
}