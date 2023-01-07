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

}

