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

}  // namespace


