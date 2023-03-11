//#PKG-CONFIG=gtest_main
#include "dictionary.hpp"
#include "std_string.hpp"
#include <string>
#include <sstream>

#include <gtest/gtest.h>
namespace
{

TEST(dict, roundtripword)
{
    Stack stack;
    ReturnStack returnStack;
    Dictionary dictionary;

    std::ostringstream oss;
    dictionary.populateDictionary(oss);

    // TODO rewrite this test in Fth
    const std::string testword{"EMIT"};
    stringToStack(stack, testword);
    addressOf(stack, returnStack, dictionary);
    wordOf(stack, returnStack, dictionary);
    const std::string str = stackToString(stack);
    EXPECT_EQ(str, testword);
}

}

