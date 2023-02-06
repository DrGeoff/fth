//#PKG-CONFIG=gtest_main

#include "test_utils.hpp"
#include <gtest/gtest.h>


namespace
{

TEST(immediate_mode, addition)
{
    const std::string program = R"(40 2 + .)";
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"42");
}

TEST(immediate_mode, twostar)
{
    const std::string program = R"(40 2 + EMIT 42 EMIT CR)";
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"**\n");
}

TEST(immediate_mode, stringquote)
{
    const std::string program = R"(." one two "
.S
)";
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"<8> 7 111 110 101 32 116 119 111\n");
}

TEST(immediate_mode, charword)
{
    const std::string program = R"([CHAR] hello
.S
)";
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"<6> 5 104 101 108 108 111\n");
}

}  // namespace

