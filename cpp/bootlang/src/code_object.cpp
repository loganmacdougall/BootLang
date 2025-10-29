#include "code_object.hpp"

CodeObject::CodeObject(const std::string&& name,
              const std::string&& doc,
              const std::vector<std::string>&& parameters,
              FunctionContext&& context)
: name(std::move(name)), doc(std::move(doc)), 
parameters(std::move(parameters)), context(std::move(context)) {}

std::string CodeObject::toDisassembly() const {
    std::ostringstream out;

    out << "Disassembly of code object (name \"" << name << "\")";
    out << context.toDisassembly();

    return out.str();
}