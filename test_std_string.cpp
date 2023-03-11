//#PKG-CONFIG=gtest_main
#include "gtest/gtest.h"

#include "std_string.hpp"
#include <string>

namespace
{

inline void stringToStackTestImpl(std::vector<int>& aStack)
{
    const std::string expected{"Hello World!"};
    stringToStack(aStack, expected);
    const std::string str = stackToString(aStack);
    EXPECT_EQ(str, expected);
}
    
TEST(str, basic)
{
    Stack stack;
    ReturnStack returnStack;
    Dictionary dictionary;
    stringToStackTestImpl(stack);
    stringToStackTestImpl(returnStack);
}

}

