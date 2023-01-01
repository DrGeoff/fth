#include <vector>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <functional>
#include "string_utils.hpp"

using Stack = std::vector<int>;
using ReturnStack = std::vector<int>;
using FthFunc = void(Stack& stack, ReturnStack& returnStack);
using Dictionary = std::unordered_map<std::string, std::function<FthFunc>>;

void number(Stack& stack, ReturnStack& returnStack, const std::string& token)
{
    const int value = std::stoi(token);
    stack.push_back(value);
}

void emit(Stack& stack, ReturnStack& returnStack)
{
    std::cout << static_cast<char>(stack.back());
    // TODO: Does emit automatically remove data from the stack?
    stack.pop_back();
}

void cr(Stack& stack, ReturnStack& returnStack)
{
    std::cout << std::endl;
}

void include(Stack& stack, ReturnStack& returnStack)
{
    // TODO
}


void add(Stack& stack, ReturnStack& returnStack)
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

struct NewCommandTokens
{
    std::string name;
    std::vector<std::string> tokens;    

    void add(const std::string& token)
    {
        if (name.empty())
        {
            name = token;
        }
        else
        {
            tokens.push_back(token);
        }
    }
};


void addNewCommandTokensToDictionary(const std::unique_ptr<NewCommandTokens>& pNewCommandTokens, Dictionary& dictionary)
{
    std::vector<std::function<FthFunc>> newCommandFuncs;

    for (const std::string& token : pNewCommandTokens->tokens)
    {
        const auto& dictionaryIt = dictionary.find(token);
        if (dictionaryIt == dictionary.end())
        {
            auto deferredNumber = [token](Stack& stack, ReturnStack& returnStack){number(stack, returnStack, token);};
            newCommandFuncs.push_back(deferredNumber);
        }
        else
        {
            newCommandFuncs.push_back(dictionaryIt->second);
        }
    }

    auto implementation = [newCommandFuncs](Stack& stack, ReturnStack& returnStack)
    {
        for (auto& func : newCommandFuncs)
        {
            func(stack, returnStack);
        }
    };

    dictionary.emplace(pNewCommandTokens->name, implementation);
}


void populateDictionary(Dictionary& dictionary)
{
    dictionary.emplace(std::string{"+"}, &add);
    dictionary.emplace(std::string{"EMIT"},&emit);
    dictionary.emplace(std::string{"CR"},&cr);
}

void processImmediateMode(Dictionary& dictionary,Stack& stack, ReturnStack& returnStack, const std::string& token)
{
    //std::cout << "Token: " << token << std::endl;
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
    Dictionary dictionary;
    populateDictionary(dictionary);

    bool immediateMode = true;
    const std::string program = R"(: 2STAR 40 2 + EMIT 42 EMIT CR ;
    2STAR)";
    std::istringstream iss(program);
    std::string line;
    while (std::getline(iss, line))
    {
        //std::cout << "Line: " << line << std::endl;
        std::istringstream issline(line);
        std::string token;
        std::unique_ptr<NewCommandTokens> pNewCommandTokens;
        while (std::getline(issline, token, ' '))
        {
            trim(token);
            //std::cout << "Parsing Token: " << token << std::endl;
            if (token.empty())
            {
                continue;
            }
            if (token == ":") 
            {
                pNewCommandTokens = std::make_unique<NewCommandTokens>();
                immediateMode = false; 
            }
            else if (token == ";") 
            { 
                addNewCommandTokensToDictionary(pNewCommandTokens, dictionary);
                pNewCommandTokens.reset();
                immediateMode = true;
            }
            else
            {
                if (immediateMode)
                {
                    processImmediateMode(dictionary, stack, returnStack, token);
                }
                else
                {
                    pNewCommandTokens->add(token);
                }
            }
        }
    }
    return 0;
}
