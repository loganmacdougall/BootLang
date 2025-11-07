#include "virtual_machine.hpp"
#include "operations/list.hpp"

CallFrame::CallFrame(std::shared_ptr<Context> context, size_t bp, bool top_level)
: context(context), vars(context->names.size()),
freevars(context->names.size()), cellvars(context->names.size()),
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
    next_f->parent = f;
    f = next_f;
  } else {
    f = next_f;
  }

  frame_count += 1;
}

void VirtualMachine::popFrame() {
  if (!f) {
    return;
  }

  frame_count -= 1;

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

void VirtualMachine::setInstructionLimit(size_t insrtuction_limit) {
  this->insrtuction_limit = insrtuction_limit;
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

void VirtualMachine::runProgram(bool reset_global) {
  if (program == nullptr) {
    throw std::runtime_error("Attempted to run program before load");
  }

  while (f && !f->top_level) {
    popFrame();
  }

  if (!f) {
    f = global;
  }

  frame_count = 1;
  instructions_ran = 0;

  if (reset_global) {
    global->reset();
  }
  
  run();
}

Value::Ptr VirtualMachine::runProgramFunction(std::string name, bool reset_global) {
  if (program == nullptr) {
    throw std::runtime_error("Attempted to run program before load");
  }
  
  auto it = program->func_map.find(name);
  
  if (it == program->func_map.end()) {
    return nullptr;
  }

  while (f) {
    popFrame();
  }

  if (reset_global) {
    global->reset();
  }

  const std::shared_ptr<CodeObject> code = program->funcs->at(it->second);

  f = new CallFrame(code->context, 0);
  
  frame_count = 1;
  instructions_ran = 0;

  return run();
}

Value::Ptr VirtualMachine::run() {
  if (program == nullptr) {
    throw std::runtime_error("Attempted to run program before load");
  }

  if (f == nullptr || global == nullptr) {
    throw std::runtime_error("Attempted to run with incomplete call stack");
  }

  size_t base_frame_count = frame_count;

  Value::Ptr result = nullptr;

  while (base_frame_count <= frame_count && instructions_ran < insrtuction_limit) {
    const Instruction& inst = f->fetch();
    runInstruction(inst);
    instructions_ran += 1;
  }

  if (instructions_ran >= insrtuction_limit) {
    throw std::runtime_error("Exceeded the instruction execution limit");
  }

  return popStack();
}

Value::Ptr VirtualMachine::runCallable(Value::Ptr func_value, Value::Ptr self, std::vector<Value::Ptr>&& args) {
  if (func_value->type == Value::Type::BUILTIN_FUNCTION) {
    auto builtin_func = Value::toDerived<BuiltinFunctionValue>(func_value);

    bool range_one = builtin_func->min_args == builtin_func->max_args;

    if (builtin_func->min_args != -1 && (size_t)builtin_func->min_args > args.size()) {
      std::stringstream err;
      err << "Called function with " << args.size() << " arguments when expected " << builtin_func->min_args;
      if (!range_one) err << " to " << (builtin_func->max_args == -1 ? "unlimited" : std::to_string(builtin_func->max_args));
      err << " arguments";
      throw std::runtime_error(err.str());
    }

    if (builtin_func->max_args != -1 && (size_t)builtin_func->max_args < args.size()) {
      std::stringstream err;
      err << "Called function with " << args.size() << " arguments when expected " << builtin_func->min_args;
      if (!range_one) err << " to " << builtin_func->max_args;
      err << " arguments";
      throw std::runtime_error(err.str());
    }
    
    Value::CallableInfo info(
      func_value, self, std::move(args), [this](Value::Ptr func_value, Value::Ptr self, std::vector<Value::Ptr>&& args) {
        return this->runCallable(func_value, self, std::move(args));
      }
    );

    return func_value->call(info);
  }

  if (func_value->type != Value::Type::FUNCTION) {
    throw std::runtime_error("Attempted to call non-function value");
  }

  std::shared_ptr<FunctionValue> func = Value::toDerived<FunctionValue>(func_value);
  std::shared_ptr<CodeObject> code = func->code;

  if (code->parameters.size() != args.size()) {
      std::stringstream err;
      err << "Called function with " << args.size() << " arguments when expected ";
      err << code->parameters.size() << " arguments";
      throw std::runtime_error(err.str());
  }

  pushFrame(code->context);
  f->vars.resize(code->context->names.size());

  for (size_t i = 0; i < args.size() && i < f->vars.size(); ++i) {
    f->vars[i] = args[i];
  }

  if (!func->freevars.empty()) {
    f->freevars = func->freevars;
  }

  if (self->type != Value::Type::NONE && code->parameters.front() == "self") {
    auto it = f->context->getNameId("self");
    if (it != Context::NOT_FOUND) {
      f->vars[it] = self;
    }
  }

  return run();
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
  std::string builtin_name = program->context->builtins[arg];

  auto it = env.builtins.find(builtin_name);

  if (it == env.builtins.end()) {
    throw std::runtime_error("Variable \"" + builtin_name + "\" is undefined");
  }

  pushStack(it->second);
}

void VirtualMachine::runLoadFast(size_t arg) {
  pushStack(f->vars[arg]);
}

void VirtualMachine::runLoadAttr(size_t arg) {
    // 1. Get attribute name from program constants
    const std::string& attr_name = f->context->names[arg];

    Value::Ptr obj = popStack();

    auto builtin_func = env.getAttribute(obj->type, attr_name);
    if (!builtin_func) {
        std::ostringstream err;
        err << "AttributeError: type '" << obj->type
            << "' has no attribute '" << attr_name << "'";
        throw std::runtime_error(err.str());
    }

    pushStack(builtin_func);
}

void VirtualMachine::runLoadIndex(size_t arg) {
  (void)arg;
  Value::Ptr index = popStack();
  Value::Ptr collection = popStack();
  
  Value::Ptr value = collection_index(collection, index);
  pushStack(value);
}

void VirtualMachine::runLoadDeref(size_t arg) {
  pushStack(f->freevars[arg]);
}

void VirtualMachine::runStoreGlobal(size_t arg) {
  Value::Ptr value = popStack();
  global->vars[arg] = Value::copy(value);
}

void VirtualMachine::runStoreFast(size_t arg) {
  Value::Ptr value = popStack();
  f->vars[arg] = Value::copy(value);
}

void VirtualMachine::runStoreAttr(size_t arg) {
  (void)arg;
  // TODO
}

void VirtualMachine::runStoreIndex(size_t arg) {
  (void)arg;
  Value::Ptr index = popStack();
  Value::Ptr collection = popStack();
  Value::Ptr value = popStack();

  collection_index_assign(collection, index, value);
}

void VirtualMachine::runStoreDeref(size_t arg) {
  Value::Ptr value = popStack();
  f->freevars[arg] = Value::copy(value);
}

void VirtualMachine::runBinaryOp(size_t arg) {
  Value::Ptr rhs = popStack();
  Value::Ptr lhs = popStack();
  Token::Type op = static_cast<Token::Type>(arg);

  auto func = env.opRegistry.get(lhs->type, rhs->type, op);

  if (!func.has_value()) {
    env.opRegistry.throwNotFunction(lhs->type, rhs->type, op);
  }

  Value::Ptr value = func.value()(lhs, rhs);

  if (value) {
    pushStack(value);
  }
}

void VirtualMachine::runUnaryOp(size_t arg) {
  Value::Ptr rhs = popStack();
  Token::Type op = static_cast<Token::Type>(arg);

  auto func = env.opRegistry.get(rhs->type, op);

  if (!func.has_value()) {
    env.opRegistry.throwNotFunction(rhs->type, op);
  }

  Value::Ptr value = func.value()(rhs);

  if (value) {
    pushStack(value);
  }
}

void VirtualMachine::runUnpackSequence(size_t arg) {
  Value::Ptr collection = popStack();

  if (collection->len() != arg) {
    std::ostringstream out;
    out << "Attempted to unpack collection of length " << collection->len() << ". ";
    out << "Expected length of " << arg << ".";
    throw std::runtime_error(out.str());
  }

  stack.resize(stack.size() + arg);

  auto iter = collection->toIter();
  for (size_t i = 0; i < arg; i++) {
    stack[stack.size() - i - 1] = collection->next(iter);
  }
}

void VirtualMachine::runJump(size_t arg) {
  f->ip += arg;
}

void VirtualMachine::runJumpBackwards(size_t arg) {
  f->ip -= arg;
}

void VirtualMachine::runJumpIfFalse(size_t arg) {
  (void)arg;
  Value::Ptr value = popStack();
  if (!value->toBool()) {
    f->ip += arg;
  }
}

void VirtualMachine::runCall(size_t arg) {
  std::vector<Value::Ptr> args(arg);
  for (size_t i = 0; i < arg; ++i) {
    args[arg - 1 - i] = popStack();
  }

  Value::Ptr self = popStack();

  Value::Ptr func_value = popStack();

  Value::Ptr result = runCallable(func_value, self, std::move(args));

  pushStack(result);
}

void VirtualMachine::runToIter(size_t arg) {
  (void)arg;
  Value::Ptr value = popStack();

  auto iter_state = value->toIter();
  auto iter_value = std::make_shared<IterableValue>(value, iter_state);

  pushStack(iter_value);
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
    pushStack(value);
    pushStack(iter->next());
  }
}

void VirtualMachine::runBuildTuple(size_t arg) {
  std::vector<Value::Ptr> elems;
  elems.resize(arg);

  long size = static_cast<long>(arg);

  for (long i = size - 1; i >= 0; i--) {
    elems[static_cast<size_t>(i)] = std::move(popStack());
  }

  Value::Ptr value = std::make_shared<TupleValue>(std::move(elems));
  pushStack(value);
}

void VirtualMachine::runBuildList(size_t arg) {
  std::vector<Value::Ptr> elems;
  elems.resize(arg);

  long size = static_cast<long>(arg);

  for (long i = size - 1; i >= 0; i--) {
    elems[static_cast<size_t>(i)] = std::move(popStack());
  }

  Value::Ptr value = std::make_shared<ListValue>(std::move(elems));
  pushStack(value);
}

void VirtualMachine::runBuildMap(size_t arg) {
  std::vector<std::pair<Value::Ptr, Value::Ptr>> pairs;
  pairs.resize(arg);

  long size = static_cast<long>(arg);

  for (long i = size - 1; i >= 0; i--) {
    Value::Ptr value = popStack();
    Value::Ptr key = popStack();
    auto pair = std::pair<Value::Ptr, Value::Ptr>(key, value);
    pairs[i] = pair;
  }

  Value::Ptr value = std::make_shared<MapValue>(std::move(pairs));
  pushStack(value);
}

void VirtualMachine::runBuildSet(size_t arg) {
  std::vector<Value::Ptr> elems;
  elems.resize(arg);

  long size = static_cast<long>(arg);

  for (long i = size - 1; i >= 0; i--) {
    elems[static_cast<size_t>(i)] = std::move(popStack());
  }

  Value::Ptr value = std::make_shared<SetValue>(std::move(elems));
  pushStack(value);
}

void VirtualMachine::runBuildSlice(size_t arg) {
  (void)arg;
  std::optional<long> start = std::nullopt;
  std::optional<long> end = std::nullopt;
  std::optional<long> step = std::nullopt;

  Value::Ptr step_value = popStack();
  if (step_value->type != Value::Type::NONE) {
    if (step_value->type != Value::Type::INT) {
      throw std::runtime_error("Slice arguments must be of type int");
    }
    step.emplace(Value::toDerived<IntValue>(step_value)->value);
  }

  Value::Ptr end_value = popStack();
  if (end_value->type != Value::Type::NONE) {
    if (end_value->type != Value::Type::INT) {
      throw std::runtime_error("Slice arguments must be of type int");
    }
    end.emplace(Value::toDerived<IntValue>(end_value)->value);
  }

  Value::Ptr start_value = popStack();
  if (start_value->type != Value::Type::NONE) {
    if (start_value->type != Value::Type::INT) {
      throw std::runtime_error("Slice arguments must be of type int");
    }
    start.emplace(Value::toDerived<IntValue>(start_value)->value);
  }

  Value::Ptr slice = std::make_shared<SliceValue>(start, end, step);
  pushStack(slice);
}

void VirtualMachine::runMakeClosure(size_t arg) {
  (void)arg;
  Value::Ptr func_value = popStack();
  Value::Ptr cells_value = popStack();

  if (func_value->type != Value::Type::FUNCTION) {
    throw std::runtime_error("Make closure expects a function value as first argument");
  }

  if (cells_value->type != Value::Type::TUPLE) {
    throw std::runtime_error("Make closure expects a tuple value as second argument");
  }

  std::shared_ptr<FunctionValue> func = Value::toDerived<FunctionValue>(func_value);
  std::shared_ptr<TupleValue> cells = Value::toDerived<TupleValue>(cells_value);

  func->setFreeVars(cells->elems);

  pushStack(func_value);
}

void VirtualMachine::runPushNull(size_t arg) {
  (void)arg;
  pushStack(NoneValue::NONE);
}

void VirtualMachine::runPopTop(size_t arg) {
  (void)arg;
  popStack();
}

void VirtualMachine::runSwap(size_t arg) {
  const size_t l = stack.size();
  Value::Ptr value1 = stack[l-1];
  Value::Ptr value2 = stack[l-1-arg];

  stack[l-1] = value2;
  stack[l-1-arg] = value1;
}

void VirtualMachine::runCopy(size_t arg) {
  Value::Ptr value = stack.back();
  for (size_t i = 0; i < arg; i++) {
    pushStack(value);
  }
}

void VirtualMachine::runPause(size_t arg) {
  (void)arg;
  // TODO later for debugging
}

void VirtualMachine::runReturn(size_t arg) {
  (void)arg;

  Value::Ptr returned = popStack();
  stack.resize(f->bp);
  pushStack(returned);

  popFrame();
}