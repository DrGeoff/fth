#pragma once

#include "stack.hpp"
#include "dictionary.hpp"
#include <ostream>

inline void dot(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, std::ostream& os)
{
    os << stack.back();
    stack.pop_back();
}

inline void emit(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, std::ostream& os)
{
    os << static_cast<char>(stack.back());
    stack.pop_back();
}

inline void cr(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, std::ostream& os)
{
    os << std::endl;
}


void include(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);

