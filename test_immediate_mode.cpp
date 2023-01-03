#include "process.hpp"
#include "stack.hpp"
#include "io.hpp"
#include "math.hpp"
#include <sstream>
#include <iostream>
#include <cassert>

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

void test_immediate_mode()
{
    const std::string program = R"(40 2 + EMIT 42 EMIT CR)";
    const std::string output = harness(program);
    assert(output == "**\n");
}


int main(int argc, char* argv[])
{
    test_immediate_mode();
    return 0;
}

