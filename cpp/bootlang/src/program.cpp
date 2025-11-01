#include "program.hpp"

Program::Program(const Program& other)
: context(other.context), funcs(other.funcs),
  line_numbers(other.line_numbers),
  func_map(other.func_map) {}

Program& Program::operator=(const Program& other) {
    if (this != &other) return *this;

    context = other.context;
    funcs = other.funcs;
    line_numbers = other.line_numbers;
    func_map = other.func_map;

    return *this;
}

Program::Program(std::shared_ptr<TopLevelContext> context,
    std::shared_ptr<std::vector<std::shared_ptr<CodeObject>>> funcs,
    std::shared_ptr<std::unordered_map<size_t, size_t>> line_numbers)
    : context(context), funcs(funcs), line_numbers(line_numbers) {
        for (size_t i = 0; i < funcs->size(); i++) {
            func_map[funcs->at(i)->name] = i;
        }
    }

std::string Program::toDisassembly() const {
    std::ostringstream out;

    out << context->toDisassembly() << std::endl;

    for (auto it = funcs->begin(); it != funcs->end(); it++) {
        auto code = *it;
        out << code->toDisassembly() << std::endl;
    }

    return out.str();
}