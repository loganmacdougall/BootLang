#include "virtual_machine.hpp"
#include "operations/list.hpp"

CallFrame::CallFrame(std::shared_ptr<Context> context, size_t bp, bool top_level)
: context(context), vars(context->vars.size()),
freevars(context->vars.size()), cellvars(context->vars.size()),
bp(bp), top_level(top_level) {}

Instruction CallFrame::fetch() {
  return context->instructions[ip++];
}

void CallFrame::rewind() {
  ip = 0;
}

void CallFrame::reset() {
  ip = 0;
  vars.clear();
  freevars.clear();
  cellvars.clear();
}

VirtualMachine::VirtualMachine(Environment& env) : 
  env(env) {}

void VirtualMachine::pushFrame(std::shared_ptr<Context> context) {
  CallFrame* next_f = new CallFrame(context, stack.size());
  if (f) {
    f->parent = next_f;
    f = f->parent;
  } else {
    f = next_f;
  }
}

void VirtualMachine::popFrame() {
  if (!f) {
    return;
  }

  if (!f->parent) {
    if (!f->top_level) {
      delete f;
    }
    f = nullptr;
    return;
  }
  
  CallFrame* next_f = f->parent;
  delete f;
  f = next_f;
}

void VirtualMachine::pushStack(Value::Ptr value) {
  stack.push_back(value);
}

Value::Ptr VirtualMachine::popStack() {
  Value::Ptr value = stack.back();
  stack.pop_back();
  return value;
}

void VirtualMachine::loadProgram(Program& program) {
  this->program = new Program(program);
  stack.clear();
  global = new CallFrame(program.context, 0, true);
}

void VirtualMachine::runProgram() {
  if (program == nullptr) {
    throw std::runtime_error("Attempted to run program before load");
  }

  while (f && !f->top_level) {
    popFrame();
  }

  if (!f) {
    f = global;
  }

  f->reset();
  
  run();
}

bool VirtualMachine::runProgramFunction(std::string name, bool reset_global) {
  if (program == nullptr) {
    throw std::runtime_error("Attempted to run program before load");
  }
  
  auto it = program->func_map.find(name);
  
  if (it == program->func_map.end()) {
    return false;
  }

  while (f) {
    popFrame();
  }

  if (reset_global) {
    global->reset();
  }

  const std::shared_ptr<CodeObject> code = program->funcs->at(it->second);


  f = new CallFrame(code->context, 0);
  run();
  
  return true;
}

void VirtualMachine::run() {
  if (program == nullptr) {
    throw std::runtime_error("Attempted to run program before load");
  }

  if (f == nullptr || global == nullptr) {
    throw std::runtime_error("Attempted to run with incomplete call stack");
  }

  Value::Ptr result = nullptr;

  while (f) {
    if (f->ip >= f->context->instructions.size()) {
      popFrame();
      continue;
    }

    const Instruction& inst = f->fetch();
    runInstruction(inst);
  }
}

void VirtualMachine::runInstruction(const Instruction& instruction) {
  using I = Instruction::Type;

  switch (instruction.type) {
    case I::LOAD_CONST:       runLoadConst(instruction.arg); break;
    case I::LOAD_INT:         runLoadInt(instruction.arg); break;
    case I::LOAD_GLOBAL:      runLoadGlobal(instruction.arg); break;
    case I::LOAD_BUILTIN:     runLoadBuiltin(instruction.arg); break;
    case I::LOAD_FAST:        runLoadFast(instruction.arg); break;
    case I::LOAD_ATTR:        runLoadAttr(instruction.arg); break;
    case I::LOAD_INDEX:       runLoadIndex(instruction.arg); break;
    case I::LOAD_DEREF:       runLoadDeref(instruction.arg); break;

    case I::STORE_GLOBAL:     runStoreGlobal(instruction.arg); break;
    case I::STORE_FAST:       runStoreFast(instruction.arg); break;
    case I::STORE_ATTR:       runStoreAttr(instruction.arg); break;
    case I::STORE_INDEX:      runStoreIndex(instruction.arg); break;
    case I::STORE_SLICE:      runStoreSlice(instruction.arg); break;
    case I::STORE_DEREF:      runStoreDeref(instruction.arg); break;

    case I::BINARY_OP:        runBinaryOp(instruction.arg); break;
    case I::UNARY_OP:         runUnaryOp(instruction.arg); break;
    case I::UNPACK_SEQUENCE:  runUnpackSequence(instruction.arg); break;

    case I::JUMP:             runJump(instruction.arg); break;
    case I::JUMP_BACKWARDS:   runJumpBackwards(instruction.arg); break;
    case I::JUMP_IF_FALSE:    runJumpIfFalse(instruction.arg); break;

    case I::CALL:             runCall(instruction.arg); break;
    case I::TO_ITER:          runToIter(instruction.arg); break;
    case I::FOR_ITER:         runForIter(instruction.arg); break;

    case I::BUILD_TUPLE:      runBuildTuple(instruction.arg); break;
    case I::BUILD_LIST:       runBuildList(instruction.arg); break;
    case I::BUILD_MAP:        runBuildMap(instruction.arg); break;
    case I::BUILD_SET:        runBuildSet(instruction.arg); break;
    case I::BUILD_SLICE:      runBuildSlice(instruction.arg); break;

    case I::MAKE_CLOSURE:     runMakeClosure(instruction.arg); break;

    case I::PUSH_NULL:        runPushNull(instruction.arg); break;
    case I::POP_TOP:          runPopTop(instruction.arg); break;
    case I::SWAP:             runSwap(instruction.arg); break;
    case I::COPY:             runCopy(instruction.arg); break;

    case I::PAUSE:            runPause(instruction.arg); break;
    case I::RETURN:           runReturn(instruction.arg); break;

    default:
      throw std::runtime_error("Unknown instruction type in VirtualMachine::runInstruction");
  }
}


void VirtualMachine::runLoadConst(size_t arg) {
  Value::Ptr value = f->context->constants[arg];
  stack.push_back(std::move(value->clone()));
}

void VirtualMachine::runLoadInt(size_t arg) {
  stack.push_back(std::make_shared<IntValue>(IntValue(arg)));
}

void VirtualMachine::runLoadGlobal(size_t arg) {
  pushStack(global->vars[arg]);
}

void VirtualMachine::runLoadBuiltin(size_t arg) {
  // TODO:
}

void VirtualMachine::runLoadFast(size_t arg) {
  pushStack(f->vars[arg]);
}

void VirtualMachine::runLoadAttr(size_t arg) {
  // TODO:
}

void VirtualMachine::runLoadIndex(size_t arg) {
  runBinaryOp(Token::LBRACE);
}

void VirtualMachine::runLoadDeref(size_t arg) {
  pushStack(f->freevars[arg]);
}

void VirtualMachine::runStoreGlobal(size_t arg) {
  Value::Ptr value = popStack();
  global->vars[arg] = value->isPrimitive() ? value->clone() : value;
}

void VirtualMachine::runStoreFast(size_t arg) {
  Value::Ptr value = popStack();
  f->vars[arg] = value->isPrimitive() ? value->clone() : value;
}

void VirtualMachine::runStoreAttr(size_t arg) {

}

void VirtualMachine::runStoreIndex(size_t arg) {

}

void VirtualMachine::runStoreSlice(size_t arg) {

}

void VirtualMachine::runStoreDeref(size_t arg) {
  Value::Ptr value = popStack();
  f->freevars[arg] = value->isPrimitive() ? value->clone() : value;
}

void VirtualMachine::runBinaryOp(size_t arg) {
  Value::Ptr collection = popStack();
  Value::Ptr index = popStack();
  Token::Type op = static_cast<Token::Type>(arg);

  auto func = env.binaryOpRegistry.get(collection->type, index->type, op);

  if (!func.has_value()) {
    env.binaryOpRegistry.throwNotFunction(collection->type, index->type, op);
  }

  Value::Ptr value = func.value()(collection, index);

  if (value) {
    pushStack(std::move(value));
  }
}

void VirtualMachine::runUnaryOp(size_t arg) {
  // Add Unary Registry or make BinaryOpRegistry or generel
}

void VirtualMachine::runUnpackSequence(size_t arg) {
  Value::Ptr collection = popStack();

  if (!collection->hasLength()) {
    std::ostringstream out;
    out << "Type " << collection->type << " isn't of type collectable.";
    throw std::runtime_error(out.str());
  }

  if (collection->len() != arg) {
    std::ostringstream out;
    out << "Attempted to unpack collection of length " << collection->len() << ". ";
    out << "Expected length of " << arg << ".";
    throw std::runtime_error(out.str());
  }

  auto iter = collection->iterInitialState();
  while (!iter->finished) {
    pushStack(std::move(collection->nextFromIter(iter)));
  }
}

void VirtualMachine::runJump(size_t arg) {
  f->ip += arg;
}

void VirtualMachine::runJumpBackwards(size_t arg) {
  f->ip -= arg;
}

void VirtualMachine::runJumpIfFalse(size_t arg) {
  Value::Ptr value = popStack();
  if (!value->toBool()) {
    f->ip += arg;
  }
}

void VirtualMachine::runCall(size_t arg) {
  // TODO
}

void VirtualMachine::runToIter(size_t arg) {
  Value::Ptr value = popStack();

  if (!value->isIterable()) {
    std::ostringstream out;
    out << "Type " << value->type << " isn't iterable";
    throw std::runtime_error(out.str());
  }

  auto iter_state = value->iterInitialState();
  auto iter_value = std::make_shared<IterableValue>(value, std::move(iter_state));

  pushStack(std::move(iter_value));
}

void VirtualMachine::runForIter(size_t arg) {
  Value::Ptr value = popStack();

  if (value->type != Value::ITERATOR) {
    throw std::runtime_error("Attempted to use non-iterator as an iterator");
  }

  std::shared_ptr<IterableValue> iter = Value::toDerived<IterableValue>(value);

  if (iter->state->finished) {
    f->ip += arg;
  } else {
    pushStack(std::move(value));
    pushStack(std::move(iter->next()));
  }
}

void VirtualMachine::runBuildTuple(size_t arg) {
  std::vector<Value::Ptr> elems;

  for (size_t i = 0; i < arg; i++) {
    elems.push_back(std::move(popStack()));
  }

  Value::Ptr value = std::make_shared<TupleValue>(std::move(elems));
  pushStack(std::move(value));
}

void VirtualMachine::runBuildList(size_t arg) {
  std::vector<Value::Ptr> elems;

  for (size_t i = 0; i < arg; i++) {
    elems.push_back(std::move(popStack()));
  }

  Value::Ptr value = std::make_shared<ListValue>(std::move(elems));
  pushStack(std::move(value));
}

void VirtualMachine::runBuildMap(size_t arg) {
  std::map<Value::Ptr, Value::Ptr> map;

  for (size_t i = 0; i < arg; i++) {
    Value::Ptr key = popStack();
    Value::Ptr value = popStack();
    map.insert({{key, value}});
  }

  Value::Ptr value = std::make_shared<MapValue>(std::move(map));
  pushStack(std::move(value));
}

void VirtualMachine::runBuildSet(size_t arg) {
  std::set<Value::Ptr> elems;

  for (size_t i = 0; i < arg; i++) {
    elems.insert(std::move(popStack()));
  }

  Value::Ptr value = std::make_shared<SetValue>(std::move(elems));
  pushStack(std::move(value));
}

void VirtualMachine::runBuildSlice(size_t arg) {
  if (arg != 2 || arg != 3) {
    throw std::runtime_error("Invalid argument provided from BUILD_SLICE.");
  }

  long start, end, step = 1;

  Value::Ptr start_value = popStack();
  Value::Ptr end_value = popStack();

  if (start_value->type != Value::Type::INT || end_value->type != Value::Type::INT) {
    throw std::runtime_error("Slice arguments must be of type int");
  }

  start = Value::toDerived<IntValue>(start_value)->value;
  end = Value::toDerived<IntValue>(end_value)->value;

  if (arg == 3) {
    Value::Ptr step_value = popStack();
    if (step_value->type != Value::Type::INT) {
      throw std::runtime_error("Slice arguments must be of type int");
    }
    step = Value::toDerived<IntValue>(step_value)->value;
  }

  Value::Ptr slice = std::make_shared<SliceValue>(start, end, step);
  pushStack(slice);

}

void VirtualMachine::runMakeClosure(size_t arg) {

}

void VirtualMachine::runPushNull(size_t arg) {

}

void VirtualMachine::runPopTop(size_t arg) {

}

void VirtualMachine::runSwap(size_t arg) {

}

void VirtualMachine::runCopy(size_t arg) {

}

void VirtualMachine::runPause(size_t arg) {

}

void VirtualMachine::runReturn(size_t arg) {

}
