#include "process.hpp"
#include "string_utils.hpp"

#include <memory>
#include <sstream>

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



void number(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, const std::string& token)
{
    const int value = std::stoi(token);
    stack.push_back(value);
}

void charToStack(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary, const std::string& token)
{
    for (const char& toke : token)
    {
        stack.push_back(static_cast<int>(toke));
    }
}

void include(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    if (stack.size() < 1)
    {
        throw std::underflow_error("Tried to include but stack size < 1");
    }

    //TODO: convert int stack into a filename to pass into fin.  ?zero sentinal
    //std::ifstream fin{stack.back()};
    stack.pop_back();
    //process(fin, dictionary, stack, returnStack);
}

void addNewCommandTokensToDictionary(const std::unique_ptr<NewCommandTokens>& pNewCommandTokens, Dictionary& dictionary)
{
    bool characterMode{false};
    bool stringMode{false};
    std::string strToken{};
    std::vector<std::function<FthFunc>> newCommandFuncs;

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
        if (token == "INCLUDE")
        {
            
        }
        else
        {
            number(stack, returnStack, dictionary, token);
        }
    }
    else
    {
        dictionaryIt->second(stack, returnStack, dictionary);
    }
}


void process(std::istream& iss, Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    bool immediateMode = true;
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
                    processImmediateMode(stack, returnStack, dictionary, token);
                }
                else
                {
                    pNewCommandTokens->add(token);
                }
            }
        }
    }
}

