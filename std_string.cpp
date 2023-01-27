#include "std_string.hpp"
#include <ranges>

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
    const int strSize = stack.back();
    stack.pop_back();
    std::string ret;
    ret.reserve(strSize);
    for (int index=0; index != strSize; ++index)
    {
        ret.push_back(stack.back());
        stack.pop_back();
    }
    return ret;
}
