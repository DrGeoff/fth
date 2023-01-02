#pragma once
#include "process.hpp"
#include <ranges>
#include <iostream>

/// Non-destructively display the stack
void dotS(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    std::cout << "<" << stack.size() << ">";
    for(const auto& element : stack|std::ranges::views::reverse)
    {
        std::cout << " " << element; 
    }
    std::cout << std::endl;
}

/// Discard the top stack item
inline void drop(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    stack.pop_back();
}

