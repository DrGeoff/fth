#include <vector>
#include <variant>
#include <iostream>
#include <unordered_map>
#include <sstream>

using Stack = std::vector<std::variant<std::string,int>>;
using ReturnStack = std::vector<std::variant<std::string,int>>;
using FthFunc = void (*)(Stack& stack, ReturnStack& returnStack);
using Dictionary = std::unordered_map<std::string, FthFunc>;

void number(Stack& stack, ReturnStack& returnStack, std::string& token)
{
    const int value = std::stoi(token);
    stack.push_back(value);
}

void emit(Stack& stack, ReturnStack& returnStack)
{
    std::cout << std::get<int>(stack.back());
}

void populateDictionary(Dictionary& dictionary)
{
    dictionary.emplace(std::string{"EMIT"},&emit);
}

int main(int argc, char* argv[])
{
    Stack stack;
    ReturnStack returnStack;
    std::unordered_map<std::string, FthFunc> dictionary;
    populateDictionary(dictionary);

    const std::string program = "5 EMIT";
    std::istringstream iss(program);
    std::string token;
    while (std::getline(iss, token, ' '))
    {
        std::cout << token << "\n";
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
    return 0;
}
