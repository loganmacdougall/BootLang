#include "program.hpp"

Program::Program(std::vector<Instruction>&& instructions, std::vector<size_t>&& line_starts)
    : instructions(std::move(instructions)), line_starts(line_starts) {}