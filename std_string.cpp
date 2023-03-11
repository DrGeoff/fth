#include "std_string.hpp"
#include <ranges>
#include <cassert>

void stringToStack(std::vector<int>& aStack, const std::string& cppstr)
{
    // Represent strings on the stack by "length c1 c2 c3 ... cn"
    // That is, not zero terminated but size prefixed.
    for (const char& charInStr : cppstr|std::ranges::views::reverse)
    {
        aStack.push_back(static_cast<int>(charInStr));
    }
    aStack.push_back(cppstr.size());
}

std::string stackToString(std::vector<int>& aStack)
{
    assert(aStack.size() > 1u);
    const unsigned int strSize = aStack.back();
    assert(aStack.size() >= strSize);
    aStack.pop_back();

    std::string ret;
    ret.reserve(strSize);
    for (unsigned int index=0; index != strSize; ++index)
    {
        ret.push_back(aStack.back());
        aStack.pop_back();
    }
    return ret;
}
