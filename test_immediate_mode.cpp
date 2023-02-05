//#PKG-CONFIG=gtest_main

#include "process.hpp"
#include "stack.hpp"
#include "io.hpp"
#include "math.hpp"
#include <sstream>
#include <iostream>
#include <gtest/gtest.h>


namespace
{

std::string harness(const std::string& program)
{
    Stack stack;
    ReturnStack returnStack;
    Dictionary dictionary;

    std::ostringstream oss;
    dictionary.populateDictionary(oss);

    std::istringstream iss{program};
    process(iss, stack, returnStack, dictionary);
    return oss.str();
}

TEST(immediate_mode, twostar)
{
    const std::string program = R"(40 2 + EMIT 42 EMIT CR)";
    const std::string output = harness(program);
    EXPECT_EQ(output,"**\n");
}

TEST(immediate_mode, stringquote)
{
    const std::string program = R"(." one two "
.S
)";
    const std::string output = harness(program);
    EXPECT_EQ(output,"<8> 7 111 110 101 32 116 119 111\n");
}

TEST(immediate_mode, charword)
{
    const std::string program = R"([CHAR] hello
.S
)";
    const std::string output = harness(program);
    EXPECT_EQ(output,"<6> 5 104 101 108 108 111\n");
}

}

