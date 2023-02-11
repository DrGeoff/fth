//#PKG-CONFIG=gtest_main

#include "test_utils.hpp"
#include <gtest/gtest.h>


namespace
{

TEST(compile_mode, fortytwo)
{
    const std::string program = R"(: fortytwo 42 ;
fortytwo
.)";
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"42");
}


TEST(compile_mode, createword)
{
    /*
    const std::string program = R"([CHAR] fortytwo [CHAR] 42 2 CREATEWORD
fortytwo
.)";
*/
    const std::string program = R"(CREATEWORD 2 42 [CHAR] fortytwo [CHAR] 
fortytwo
.)";
    const std::string output = fth::harness(program);
    EXPECT_EQ(output,"42");
}

}  // namespace


