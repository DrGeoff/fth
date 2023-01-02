#include "io.hpp"
#include <stdexcept>

void include(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    if (stack.size() < 1)
    {
        throw std::underflow_error("Tried to include but stack size < 1");
    }

    //TODO: convert int stack into a filename to pass into fin.  ?zero sentinal
    //std::ifstream fin{stack.back()};
    stack.pop_back();
    //process(fin, dictionary, stack, returnStack);
}

