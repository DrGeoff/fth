//#PKG-CONFIG=gtest_main

#include "test_utils.hpp"
#include <gtest/gtest.h>


namespace
{

TEST(immediate_mode, addition)
{
    //const std::string program = R"(40 2 + .)";   // Normal Forth
    const std::string program = R"(. + 40 2)";     // FTH
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"42");
}

TEST(immediate_mode, twostar)
{
    //const std::string program = R"(40 2 + EMIT 42 EMIT CR)";// Normal Forth
    const std::string program = R"(CR EMIT 42 EMIT + 2 40)";  // FTH
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"**\n");
}

TEST(immediate_mode, stringquote)
{
    // Reversed from normal Forth
    const std::string program = R"(" two one ."
.S
)";
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"<8> 7 111 110 101 32 116 119 111\n");
}

TEST(immediate_mode, charword)
{
    // Reversed from normal Forth
    const std::string program = R"(hello [CHAR]
.S
)";
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"<6> 5 104 101 108 108 111\n");
}

}  // namespace

