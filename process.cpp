#include "process.hpp"
#include "string_utils.hpp"
///#include "stack.hpp"   // TODO: remove this. only for debugging

#include <memory>
#include <sstream>
#include <ranges>
#include <iostream>
#include <cassert>

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

    void dump() const
    {
        std::cout << "CommandTokens. name=" << name << " tokens=";
        for (const auto& token : tokens)
        {
            std::cout << " " << token;
        }
    }
};



void number(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, const std::string& token)
{
    const int value = std::stoi(token);
    stack.push_back(value);
}

void charToStack(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, const std::string& token)
{
    // Represent strings on the stack by "length c1 c2 c3 ... cn"
    // That is, not zero terminated but size prefixed.
    for (const char& toke : token|std::ranges::views::reverse)
    {
        stack.push_back(static_cast<int>(toke));
    }
    stack.push_back(token.size());
}

void addNewCommandTokensToDictionary(const std::unique_ptr<NewCommandTokens>& pNewCommandTokens, Dictionary& dictionary)
{
    bool characterMode{false};
    bool stringMode{false};
    std::string strToken{};
    std::vector<std::function<FthFunc>> newCommandFuncs;
    /*std::cout << "addNewCommandTokensToDictionary called\n";
    pNewCommandTokens->dump();
    std::cout << std::endl;*/

    for (const std::string& token : pNewCommandTokens->tokens)
    {
        if (token=="[CHAR]")
        {
            characterMode = true;
            continue;
        }

        if (token==".\"")
        {
            stringMode = true;
            strToken = "";
            continue;
        }

        if (stringMode)
        {
            if (token[0] == '"')
            {
                stringMode = false;
            }
            else
            {
                strToken += " ";
                strToken += token;
                continue;
            }
        }

        const auto& dictionaryIt = dictionary.find(token);
        if (dictionaryIt == dictionary.end())
        {
            if (!strToken.empty())
            {
                auto deferred = [strToken](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary){charToStack(stack, returnStack, dictionary, strToken);};
                newCommandFuncs.push_back(deferred);
                strToken = "";
            }
            else if (characterMode)
            {
                auto deferred = [token](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary){charToStack(stack, returnStack, dictionary, token);};
                newCommandFuncs.push_back(deferred);
                characterMode = false;
            }
            else
            {
                auto deferredNumber = [token](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary){number(stack, returnStack, dictionary, token);};
                newCommandFuncs.push_back(deferredNumber);
            }
        }
        else
        {
            newCommandFuncs.push_back(dictionaryIt->second);
        }
    }

    auto implementation = [newCommandFuncs](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
    {
        for (auto& func : newCommandFuncs)
        {
            func(stack, returnStack, dictionary);
        }
    };

    dictionary.emplace(pNewCommandTokens->name, implementation);
}

void processImmediateMode(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, const std::string& token)
{
    //std::cout << "Token: " << token << std::endl;
    const auto& dictionaryIt = dictionary.find(token);
    if (dictionaryIt == dictionary.end())
    {
        number(stack, returnStack, dictionary, token);
    }
    else
    {
        dictionaryIt->second(stack, returnStack, dictionary);
    }
}

void executeImmediateTokens(std::unique_ptr<NewCommandTokens>& pImmediateTokens, Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    if (pImmediateTokens && !pImmediateTokens->tokens.empty())
    {
        /*
        std::cout << "executeImmediateTokens. name=" << pImmediateTokens->name << " tokens=";
        pImmediateTokens->dump();
        std::cout << std::endl;            
        dumpDictionary(stack, returnStack, dictionary); // TODO: REMOVE
        */
        addNewCommandTokensToDictionary(pImmediateTokens, dictionary);
        const auto& dictionaryIt = dictionary.find(pImmediateTokens->name);
        pImmediateTokens = std::make_unique<NewCommandTokens>();
        dictionaryIt->second(stack, returnStack, dictionary);
        dictionary.erase(dictionaryIt);
    }
}

void process(std::istream& iss, Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    static int immediateCounter{0};
    bool immediateMode = true;
    std::string line;
    while (std::getline(iss, line))
    {
        //std::cout << "Line: " << line << std::endl;
        std::istringstream issline(line);
        std::string token;
        std::unique_ptr<NewCommandTokens> pNewCommandTokens{};
        std::unique_ptr<NewCommandTokens> pImmediateTokens{};
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
                executeImmediateTokens(pImmediateTokens, stack, returnStack, dictionary);
                pImmediateTokens = std::make_unique<NewCommandTokens>();
                pNewCommandTokens = std::make_unique<NewCommandTokens>();
                immediateMode = false; 
            }
            else if (token == ";") 
            { 
                //dumpDictionary(stack, returnStack, dictionary); // TODO: REMOVE
                addNewCommandTokensToDictionary(pNewCommandTokens, dictionary);
                //dumpDictionary(stack, returnStack, dictionary); // TODO: REMOVE
                pNewCommandTokens = std::make_unique<NewCommandTokens>();
                immediateMode = true;
            }
            else
            {
                if (immediateMode)
                {
                    if (!pImmediateTokens)
                    {
                        pImmediateTokens = std::make_unique<NewCommandTokens>();
                    }
                    if (pImmediateTokens->name.empty())
                    {                        
                        pImmediateTokens->add("immediate"+std::to_string(++immediateCounter));
                    }
                    pImmediateTokens->add(token);
                }
                else
                {
                    pNewCommandTokens->add(token);
                }
            }
        }
        executeImmediateTokens(pImmediateTokens, stack, returnStack, dictionary);
    }
}

