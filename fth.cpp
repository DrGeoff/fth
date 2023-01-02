#include "process.hpp"
#include "io.hpp"
#include <sstream>


void add(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    if (stack.size() < 2)
    {
        throw std::underflow_error("Tried to add but stack size < 2");
    }

    const int value = stack.back() + stack.at(stack.size()-2);
    // TODO: Does add automatically remove data from the stack?
    stack.pop_back();
    stack.pop_back();
    // TODO: Should this go onto the returnStack?  Who/When moves from the return stack onto the stack?
    stack.push_back(value);
}

void dotS(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    std::cout << "<" << stack.size() << ">";
    for(const auto& element : stack)
    {
        std::cout << " " << element; 
    }
    std::cout << std::endl;
}

void populateDictionary(Dictionary& dictionary)
{
    dictionary.emplace(std::string{"INCLUDE"}, &include);
    dictionary.emplace(std::string{"+"}, &add);
    dictionary.emplace(std::string{"."}, &dot);
    dictionary.emplace(std::string{"EMIT"}, &emit);
    dictionary.emplace(std::string{"CR"}, &cr);
    dictionary.emplace(std::string{".S"}, &dotS);
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

    const std::string program = R"(: STAR ." *star astar* " .S EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT EMIT CR ;
    STAR)";

    std::istringstream iss(program);
    process(iss, stack, returnStack, dictionary);
    return 0;
}
