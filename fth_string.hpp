#pragma once 

// Convert a Fth string on the stack into an integer on the stack.  That is the Fth version of stoi.
inline void number(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    const std::string token = stackToString(stack, returnStack, dictionary);
    const int value = std::stoi(token);
    stack.push_back(value);
}

