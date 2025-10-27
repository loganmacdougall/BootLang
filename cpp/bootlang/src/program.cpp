#include "program.hpp"

Program::Program(TopLevelContext&& context, const Environment& env)
    : context(std::move(context)), env(env) {}

std::string Program::toDissassembly() const {
    std::ostringstream out;

    out << context.toDissassembly() << std::endl;

    return out.str();
}