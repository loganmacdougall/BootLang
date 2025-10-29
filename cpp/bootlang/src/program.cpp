#include "program.hpp"

Program::Program(TopLevelContext&& context, const Environment& env)
    : context(std::move(context)), env(env) {}

std::string Program::toDisassembly() const {
    std::ostringstream out;

    out << context.toDisassembly() << std::endl;

    return out.str();
}