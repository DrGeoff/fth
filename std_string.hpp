#pragma once
#include "stack.hpp"
#include "dictionary.hpp"
#include <string>

// Push a std::string into a Fth string on the stack
void stringToStack(std::vector<int>& aStack, const std::string& str);

// Pop a Fth string from the stack and convert to a std::string
std::string stackToString(std::vector<int>& aStack);

