#include "dictionary.hpp"
#include <iostream>

void test_dictionary()
{
    Dictionary dictionary;
    dictionary.populateDictionary(std::cout);
}

int main(int argc, char* argv[])
{
    test_dictionary();
    return 0;
}

