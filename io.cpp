#include "io.hpp"
#include "process.hpp"

#include <string>
#include <fstream>
#include <stdexcept>

std::string readStringFromStack(Stack& stack)
{
    std::string str{};
    const int length = stack.back();
    stack.pop_back();
    for (int index = 0; index != length; ++index)
    {
        str += stack.back();
        stack.pop_back();
    }
    return str;
}

void include(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    if (stack.size() < 1)
    {
        throw std::underflow_error("Tried to include but stack size < 1");
    }

    const std::string filename = readStringFromStack(stack);
    std::ifstream fin{filename};
    process(fin, stack, returnStack, dictionary);
}

