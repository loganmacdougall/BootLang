#include "program.hpp"

Program::Program(std::shared_ptr<TopLevelContext> context, const Environment& env)
    : context(context), env(env) {}

std::string Program::toDisassembly() const {
    std::ostringstream out;

    out << context->toDisassembly() << std::endl;

    return out.str();
}