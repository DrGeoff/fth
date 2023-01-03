#pragma once

#include "stack.hpp"
#include <functional>
#include <unordered_map>
#include <string>
#include <ostream>

// The purpose of some functions is to manipulate the dictionary so that must be available to all functions
struct Dictionary;
using FthFunc = void(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);
//typedef void (*FthFunc)(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);

struct Dictionary : public std::unordered_map<std::string, std::function<FthFunc>> 
{
    void populateDictionary(std::ostream& os);
};

