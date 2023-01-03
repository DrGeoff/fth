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
    //populateDictionary(dictionary, std::cout);

    std::ofstream fout{"out.txt"};
    dictionary.populateDictionary(fout);

        
    /*
    const std::string program = R"(: 2STAR 40 2 + EMIT 42 EMIT CR ;
    2STAR)";
    */
    /*
    const std::string program = R"([CHAR] 2star.fth INCLUDE
    )";
    */
    //const std::string program = R"(." 2star.fth " INCLUDE")";
     
    /*
    const std::string program = R"(: STAR [CHAR] *star* .S EMIT CR ;
    STAR)";
    */

    /*
    const std::string program = R"(: STAR ." *star astar* " .S DROP EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT CR ;
    STAR)";
    */

    //std::istringstream iss(program);
    process(std::cin, stack, returnStack, dictionary);
    return 0;
}
