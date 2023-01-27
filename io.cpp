#include "io.hpp"
#include "std_string.hpp"
#include "process.hpp"

#include <string>
#include <fstream>
#include <stdexcept>

namespace {

} // namespace

void include(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    if (stack.size() < 1)
    {
        throw std::underflow_error("Tried to include but stack size < 1");
    }

    const std::string filename = stackToString(stack, returnStack, dictionary);
    std::ifstream fin{filename};
    process(fin, stack, returnStack, dictionary);
}

