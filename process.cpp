#include "process.hpp"
#include "string_utils.hpp"
#include "std_string.hpp"
#include "fth_string.hpp"
///#include "stack.hpp"   // TODO: remove this. only for debugging
//#include <iostream>

#include <memory>
#include <sstream>
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

    /* //only used when debugging
    void dump() const
    {
        std::cout << "CommandTokens. name=" << name << " tokens=";
        for (const auto& token : tokens)
        {
            std::cout << " " << token;
        }
    }
    */
};


// TODO.  Rewrite, read the command tokens from the stack 
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

    dictionary.emplace(pNewCommandTokens->name, implementation);
}

void processImmediateMode(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, const std::string& token)
{
    //std::cout << "Token: " << token << std::endl;
    const auto& dictionaryIt = dictionary.find(token);
    if (dictionaryIt == dictionary.end())
    {
        stringToStack(stack, returnStack, dictionary, token);
        number(stack, returnStack, dictionary);
    }
    else
    {
        dictionaryIt->second(stack, returnStack, dictionary);
    }
}

// TODO.  Rewrite, read the name and the command tokens from the stack 
void executeImmediateTokens(std::unique_ptr<NewCommandTokens>& pImmediateTokens, Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    if (!pImmediateTokens || pImmediateTokens->tokens.empty())
    {
        // Nothing to do.  Probably a bug somewhere so let's get noisy
        throw std::runtime_error("executeImmediateTokens called with no tokens.  That seems a little fishy.");
    }
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

// TODO.  Rewrite, put the command tokens onto the stack 
// TODO. Use a state machine for the rewrite
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
                executeImmediateTokens(pNewCommandTokens, stack, returnStack, dictionary);
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
                    if (!pNewCommandTokens)
                    {
                        pNewCommandTokens = std::make_unique<NewCommandTokens>();
                    }
                    if (pNewCommandTokens->name.empty())
                    {                        
                        pNewCommandTokens->add("immediate"+std::to_string(++immediateCounter));
                    }
                }
                pNewCommandTokens->add(token);
            }
        }
        executeImmediateTokens(pNewCommandTokens, stack, returnStack, dictionary);
    }
}

