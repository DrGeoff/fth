#include "stack.hpp"
#include "dictionary.hpp"
#include "process.hpp"
#include "io.hpp"
#include "math.hpp"

#include <sstream>
#include <iostream>
#include <fstream>


int main(int argc, char* argv[])
{
    Stack stack;
    ReturnStack returnStack;
    Dictionary dictionary;

    dictionary.populateDictionary(std::cout);
    process(std::cin, stack, returnStack, dictionary);
    return 0;
}
