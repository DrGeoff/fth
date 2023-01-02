#include "process.hpp"
#include "stack.hpp"
#include "io.hpp"
#include "math.hpp"
#include <sstream>
#include <stdexcept>


void populateDictionary(Dictionary& dictionary)
{
    dictionary.emplace(std::string{"INCLUDE"}, &include);
    dictionary.emplace(std::string{"+"}, &add);
    dictionary.emplace(std::string{"."}, &dot);
    dictionary.emplace(std::string{"EMIT"}, &emit);
    dictionary.emplace(std::string{"CR"}, &cr);
    dictionary.emplace(std::string{".S"}, &dotS);
    dictionary.emplace(std::string{"DROP"}, &drop);
}

int main(int argc, char* argv[])
{
    Stack stack;
    ReturnStack returnStack;
    Dictionary dictionary;
    populateDictionary(dictionary);

    /*
    const std::string program = R"(: 2STAR 40 2 + EMIT 42 EMIT CR ;
    2STAR)";
    */
    //const std::string program = "2star.fth INCLUDE";
     
    /*
    const std::string program = R"(: STAR [CHAR] *star* .S EMIT CR ;
    STAR)";
    */

    const std::string program = R"(: STAR ." *star astar* " .S DROP EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT CR ;
    STAR)";

    std::istringstream iss(program);
    process(iss, stack, returnStack, dictionary);
    return 0;
}
