#pragma once

#include "stack.hpp"
#include <functional>
#include <unordered_map>
#include <string>
#include <ostream>

// The purpose of some functions is to manipulate the dictionary so that must be available to all functions
struct Dictionary;
using FthFunc = void(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);

struct Dictionary : public std::unordered_map<std::string, std::function<FthFunc>> 
{
    void populateDictionary(std::ostream& os);
};


// Forth typically calls this function `  (tick, on the tilde key)
// Pop a word (Fth function name) on the stack and push the corresponding function pointer 
void addressOf(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);

// Pop a function pointer off the stack and push the word (Fth function name) onto the stack
void wordOf(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);

