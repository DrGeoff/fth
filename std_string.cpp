#include "std_string.hpp"
#include <ranges>
#include <cassert>

void stringToStack(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, const std::string& cstr)
{
    // Represent strings on the stack by "length c1 c2 c3 ... cn"
    // That is, not zero terminated but size prefixed.
    for (const char& charInStr : cstr|std::ranges::views::reverse)
    {
        stack.push_back(static_cast<int>(charInStr));
    }
    stack.push_back(cstr.size());
}

std::string stackToString(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    assert(stack.size() > 1u);
    const unsigned int strSize = stack.back();
    assert(stack.size() >= strSize);
    stack.pop_back();

    std::string ret;
    ret.reserve(strSize);
    for (unsigned int index=0; index != strSize; ++index)
    {
        ret.push_back(stack.back());
        stack.pop_back();
    }
    return ret;
}
