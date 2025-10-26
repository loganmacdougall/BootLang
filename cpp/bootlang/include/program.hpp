#include "instruction.hpp"

class Program {
  std::vector<Instruction> instructions;
  std::vector<size_t> line_starts;

  public:
    Program(std::vector<Instruction>&& instructions, std::vector<size_t>&& line_starts);
};