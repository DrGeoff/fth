#include <vector>
#include <iostream>
#include <unordered_map>
#include <sstream>

using Stack = std::vector<int>;
using ReturnStack = std::vector<int>;
using FthFunc = void (*)(Stack& stack, ReturnStack& returnStack);
using Dictionary = std::unordered_map<std::string, FthFunc>;

void number(Stack& stack, ReturnStack& returnStack, const std::string& token)
{
    const int value = std::stoi(token);
    stack.push_back(value);
}

void emit(Stack& stack, ReturnStack& returnStack)
{
    std::cout << static_cast<char>(stack.back());
    stack.pop_back();
}

void cr(Stack& stack, ReturnStack& returnStack)
{
    std::cout << std::endl;
}

void docol(Dictionary& dictionary, Stack& stack, ReturnStack& returnStack, const std::string& token)
{
    
}

void populateDictionary(Dictionary& dictionary)
{
    dictionary.emplace(std::string{"EMIT"},&emit);
    dictionary.emplace(std::string{"CR"},&cr);
}

void processImmediateMode(Dictionary& dictionary,Stack& stack, ReturnStack& returnStack, const std::string& token)
{
    //std::cout << "Token: " << token << "\n";
    const auto& dictionaryIt = dictionary.find(token);
    if (dictionaryIt == dictionary.end())
    {
        number(stack, returnStack, token);
    }
    else
    {
        dictionaryIt->second(stack, returnStack);
    }
}

int main(int argc, char* argv[])
{
    Stack stack;
    ReturnStack returnStack;
    std::unordered_map<std::string, FthFunc> dictionary;
    populateDictionary(dictionary);

    bool immediateMode = true;
    const std::string program = "42 EMIT 42 EMIT CR";
    std::istringstream iss(program);
    std::string line;
    while (std::getline(iss, line))
    {
        std::istringstream issline(program);
        std::string token;
        while (std::getline(issline, token, ' '))
        {
            if (token == ":") { immediateMode = false; continue;}
            if (token == ";") { immediateMode = true;  continue;}
            
            if (immediateMode)
            {
                processImmediateMode(dictionary, stack, returnStack, token);
            }
            else
            {
                docol(dictionary, stack, returnStack, token);
            }
        }
    }
    return 0;
}
