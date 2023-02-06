#pragma once

#include "process.hpp"
#include "stack.hpp"
#include "dictionary.hpp"
#include <sstream>
#include <string>

namespace fth 
{

inline std::string harness(const std::string& program)
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

}

