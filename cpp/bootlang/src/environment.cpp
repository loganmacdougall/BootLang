#include "environment.hpp"

Environment::Environment()
: binaryOpRegistry(BinaryOpRegistry::GetInstance()) {}