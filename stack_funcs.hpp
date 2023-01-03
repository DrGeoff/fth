#pragma once

#include "stack.hpp"
#include "dictionary.hpp"
#include <ranges>
#include <iostream>

/// Non-destructively display the stack
inline void dotS(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, std::ostream& os)
{
    os << "<" << stack.size() << ">";
    for(const auto& element : stack|std::ranges::views::reverse)
    {
        os << " " << element; 
    }
    os << std::endl;
}

/// Discard the top stack item
inline void drop(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    stack.pop_back();
}


inline void dumpDictionary(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, std::ostream& os)
{
    for (const auto& [key,value] : dictionary)
    {
        os << key << std::endl;
    }
}

