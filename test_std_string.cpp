//#PKG-CONFIG=gtest_main
#include "std_string.hpp"
#include <string>

namespace
{

TEST(str, basic)
{
    Stack stack;
    ReturnStack returnStack;
    Dictionary dictionary;

    const std::string expected{"Hello World!"};
    stringToStack(stack, returnStack, dictionary, expected);
    const std::string str = stackToString(stack, returnStack, dictionary);
    EXPECT_EQ(str, expected);
}

}

