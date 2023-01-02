#pragma once

#include <iostream>
#include <fstream>
#include "process.hpp"

inline void dot(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    std::cout << stack.back();
    stack.pop_back();
}

inline void emit(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    std::cout << static_cast<char>(stack.back());
    stack.pop_back();
}

inline void cr(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    std::cout << std::endl;
}


void include(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);

