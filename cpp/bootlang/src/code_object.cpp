#include "code_object.hpp"

CodeObject::CodeObject(const std::string& name,
              const std::string& doc,
              const std::vector<std::string>&& parameters,
              std::shared_ptr<FunctionContext> context)
: name(name), doc(doc), 
parameters(std::move(parameters)), context(context) {}

std::string CodeObject::toDisassembly() const {
    std::ostringstream out;

    out << "Disassembly of code object \"" << name << "\":\n";
    out << context->toDisassembly();

    return out.str();
}