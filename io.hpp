#pragma once

#include <iostream>
#include <fstream>

inline void emit(Stack& stack, ReturnStack& returnStack)
{
    std::cout << static_cast<char>(stack.back());
    // TODO: Does emit automatically remove data from the stack?
    stack.pop_back();
}

inline void cr(Stack& stack, ReturnStack& returnStack)
{
    std::cout << std::endl;
}

inline void include(Stack& stack, ReturnStack& returnStack)
{
    if (stack.size() < 1)
    {
        throw std::underflow_error("Tried to include but stack size < 1");
    }

    //TODO: convert int stack into a filename to pass into fin.  ?zero sentinal
    //std::ifstream fin{stack.back()};
    stack.pop_back();
}

