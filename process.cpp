#include "process.hpp"
#include "string_utils.hpp"
#include "std_string.hpp"
#include "fth_string.hpp"
#include "stack_funcs.hpp"   // TODO: remove this. only for debugging
#include <iostream>// TODO: remove this. only for debugging

#include <ranges>
#include <memory>
#include <sstream>
#include <cassert>


void addNewCommandTokensToDictionary(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    //std::cout << "addNewCommandTokensToDictionary" << std::endl;
    bool characterMode{false};
    bool stringMode{false};
    std::string strToken{};
    std::vector<std::function<FthFunc>> newCommandFuncs;

    const int numTokens = stack.back()-1;
    assert(numTokens > 0);
    stack.pop_back();
    for (int index = 0; index != numTokens; ++index)
    {
        const std::string& token = stackToString(stack, returnStack, dictionary);
        //std::cout << "Working on token = " << token << std::endl;
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
                if (!strToken.empty())
                {
                    strToken += " ";
                }
                strToken += token;
                continue;
            }
        }

        const auto& dictionaryIt = dictionary.find(token);
        if (dictionaryIt == dictionary.end())
        {
            if (!strToken.empty())
            {
                auto deferred = [strToken](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary){stringToStack(stack, returnStack, dictionary, strToken);};
                newCommandFuncs.push_back(deferred);
                strToken = "";
            }
            else if (characterMode)
            {
                auto deferred = [token](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary){stringToStack(stack, returnStack, dictionary, token);};
                newCommandFuncs.push_back(deferred);
                characterMode = false;
            }
            else
            {
                auto deferredNumber = [token](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary){stringToStack(stack, returnStack, dictionary, token); number(stack, returnStack, dictionary);};
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

    const std::string& name = stackToString(stack, returnStack, dictionary);
    //std::cout << "Created new command " << name << std::endl;
    dictionary.emplace(name, implementation);
}

void executeImmediateTokens(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    /*
    dumpDictionary(stack, returnStack, dictionary, std::cout); // TODO: REMOVE
    */
    const std::string name = stackToString(stack, returnStack, dictionary);   
    //std::cout << "executeImmediateTokens adding new tokens to name=" << name << std::endl;
    addNewCommandTokensToDictionary(stack, returnStack, dictionary);
    // TODO: would be better if the name was at the top of the stack here rather than earlier
    //dumpDictionary(stack, returnStack, dictionary, std::cout);
    //std::cout << "executeImmediateTokens attempting to find name=" << name << std::endl;
    const auto& dictionaryIt = dictionary.find(name);
    assert(dictionaryIt!=dictionary.end());
    dictionaryIt->second(stack, returnStack, dictionary);
    dictionary.erase(dictionaryIt);
}

enum class ProcessState
{
    ImmediateMode,
    CompileMode
};

std::ostream& operator << (std::ostream& os, const ProcessState& obj)
{
   os << static_cast<std::underlying_type<ProcessState>::type>(obj);
   return os;
}

void process(std::istream& iss, Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    static int immediateCounter{0};
    int tokenCounter{0};
    ProcessState state{ProcessState::ImmediateMode};
    std::string immediateName;

    std::string line;
    while (std::getline(iss, line))
    {
        //std::cout << "Line: " << line << std::endl;
        std::istringstream issline(line);
        std::string token;
        while (std::getline(issline, token, ' '))
        {
            trim(token);
            //std::cout << "State: " << state << " Parsing Token: " << token << std::endl;
            if (token.empty())
            {
                continue;
            }
            if (tokenCounter == 0 && token != ":" && state == ProcessState::ImmediateMode) 
            {
                immediateName = "immediate"+std::to_string(++immediateCounter);
                //std::cout << "Putting name on stack as FTH string" << immediateName << std::endl;
                stringToStack(stack, returnStack, dictionary, immediateName);
            }
            else if (token == ":") 
            {
                if (tokenCounter > 0)
                {
                    stack.push_back(tokenCounter+1);
                    stringToStack(stack, returnStack, dictionary, immediateName);
                    //dotS(stack, returnStack,dictionary, std::cout);
                    //dumpDictionary(stack, returnStack, dictionary, std::cout); // TODO: REMOVE
                    executeImmediateTokens(stack, returnStack, dictionary);
                    tokenCounter = 0;
                }
                state = ProcessState::CompileMode;
                continue;
            }
            else if (token == ";") 
            { 
                //dumpDictionary(stack, returnStack, dictionary, std::cout); // TODO: REMOVE
                stack.push_back(tokenCounter);
                addNewCommandTokensToDictionary(stack, returnStack, dictionary);
                tokenCounter = 0;
                //dumpDictionary(stack, returnStack, dictionary, std::cout); // TODO: REMOVE
                state = ProcessState::ImmediateMode;
                continue;
            }
            //std::cout << "Putting token=" << token << " on stack as FTH string" << std::endl;
            stringToStack(stack, returnStack, dictionary, token);
            ++tokenCounter;
        }
        if (tokenCounter > 0)
        {
            //std::cout << "Putting tokenCounter+1=" <<tokenCounter+1 << " on stack as int" << std::endl; 
            stack.push_back(tokenCounter+1);
            //std::cout << "Putting name (again) on stack as FTH string" << immediateName << std::endl;
            stringToStack(stack, returnStack, dictionary, immediateName);
            //dotS(stack, returnStack,dictionary, std::cout);
            //dumpDictionary(stack, returnStack, dictionary, std::cout); // TODO: REMOVE
            executeImmediateTokens(stack, returnStack, dictionary);
            tokenCounter = 0;
        }
    }
}

