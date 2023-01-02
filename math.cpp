#include "math.hpp"
#include <stdexcept>

void add(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    if (stack.size() < 2)
    {
        throw std::underflow_error("Tried to add but stack size < 2");
    }

    const int value = stack.back() + stack.at(stack.size()-2);
    // TODO: Does add automatically remove data from the stack?
    stack.pop_back();
    stack.pop_back();
    // TODO: Should this go onto the returnStack?  Who/When moves from the return stack onto the stack?
    stack.push_back(value);
}

