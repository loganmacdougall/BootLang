#pragma once

#include "environment.hpp"
#include "program.hpp"

struct CallFrame {
  std::shared_ptr<Context> context;
  std::vector<Value::Ptr> vars;
  std::vector<Value::Ptr> freevars;
  std::vector<Value::Ptr> cellvars;
  size_t bp = 0;
  size_t ip = 0;
  CallFrame* parent = nullptr;

  const bool top_level;
  
  CallFrame(std::shared_ptr<Context> context, size_t bp, bool top_level = false);
  Instruction fetch();
  void rewind();
  void reset();
};

class VirtualMachine {

private:
  const Environment& env;
  Program* program = nullptr;
  CallFrame *f = nullptr, *global = nullptr;
  std::vector<Value::Ptr> stack;

public:
  VirtualMachine(Environment& env);
  void pushFrame(std::shared_ptr<Context> context);
  void popFrame();
  void pushStack(Value::Ptr value);
  Value::Ptr popStack();
  void loadProgram(Program& program);
  void runProgram();
  bool runProgramFunction(std::string name, bool global_reset);

private:
  void runContext(std::shared_ptr<Context> context);
  void run();
  void runInstruction(const Instruction& instruction);

  void runLoadConst(size_t arg);
  void runLoadInt(size_t arg);
  void runLoadGlobal(size_t arg);
  void runLoadBuiltin(size_t arg);
  void runLoadFast(size_t arg);
  void runLoadAttr(size_t arg);
  void runLoadIndex(size_t arg);
  void runLoadDeref(size_t arg);
  void runStoreGlobal(size_t arg);
  void runStoreBuiltin(size_t arg);
  void runStoreFast(size_t arg);
  void runStoreAttr(size_t arg);
  void runStoreIndex(size_t arg);
  void runStoreSlice(size_t arg);
  void runStoreDeref(size_t arg);
  void runBinaryOp(size_t arg);
  void runUnaryOp(size_t arg);
  void runUnpackSequence(size_t arg);
  void runJump(size_t arg);
  void runJumpBackwards(size_t arg);
  void runJumpIfFalse(size_t arg);
  void runCall(size_t arg);
  void runToIter(size_t arg);
  void runForIter(size_t arg);
  void runBuildTuple(size_t arg);
  void runBuildList(size_t arg);
  void runBuildMap(size_t arg);
  void runBuildSet(size_t arg);
  void runBuildSlice(size_t arg);
  void runMakeFunction(size_t arg);
  void runMakeClosure(size_t arg);
  void runPushNull(size_t arg);
  void runPopTop(size_t arg);
  void runSwap(size_t arg);
  void runCopy(size_t arg);
  void runPause(size_t arg);
  void runReturn(size_t arg);
};