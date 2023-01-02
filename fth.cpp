#include "process.hpp"
#include "stack.hpp"
#include "io.hpp"
#include "math.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>


void populateDictionary(Dictionary& dictionary, std::ostream& os)
{
    dictionary.emplace(std::string{"+"}, &add);

    //auto osDot = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dot(stack, returnStack, dictionary, os);};
    dictionary.emplace(std::string{"."}, [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dot(stack, returnStack, dictionary, os);});

    auto osEmit = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {emit(stack, returnStack, dictionary, os);};
    dictionary.emplace(std::string{"EMIT"}, osEmit);

    auto osCR = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {cr(stack, returnStack, dictionary, os);};
    dictionary.emplace(std::string{"CR"}, osCR);

    auto osDotS = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dotS(stack, returnStack, dictionary, os);};
    dictionary.emplace(std::string{".S"}, osDotS);

    //auto osDumpDictionary = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dumpDictionary(stack, returnStack, dictionary, os);};
    dictionary.emplace(std::string{"DUMPDICT"}, [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dumpDictionary(stack, returnStack, dictionary, os);});

    dictionary.emplace(std::string{"DROP"}, &drop);
    dictionary.emplace(std::string{"INCLUDE"}, &include);
}

int main(int argc, char* argv[])
{
    Stack stack;
    ReturnStack returnStack;
    Dictionary dictionary;
    //populateDictionary(dictionary, std::cout);

    std::ofstream fout{"out.txt"};
    populateDictionary(dictionary, fout);

        
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
