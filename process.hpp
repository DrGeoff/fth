#pragma once
#include "stack.hpp"
#include "dictionary.hpp"
#include <istream>

void process(std::istream& iss, Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);

void addNewCommandTokensToDictionary(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);
