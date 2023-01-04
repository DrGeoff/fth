//#PKG-CONFIG=gtest_main

#include "dictionary.hpp"
#include <iostream>

#include <gtest/gtest.h>

namespace
{

TEST(dictionary, basic)
{
    Dictionary dictionary;
    dictionary.populateDictionary(std::cout);
}

}

