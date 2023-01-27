#include "dictionary.hpp"
#include "io.hpp"
#include "math.hpp"
#include "stack_funcs.hpp"
#include "std_string.hpp"
#include <stdexcept>


void Dictionary::populateDictionary(std::ostream& os)
{
    this->emplace(std::string{"+"}, &add);
    this->emplace(std::string{"."}, [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dot(stack, returnStack, dictionary, os);});

    auto osEmit = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {emit(stack, returnStack, dictionary, os);};
    this->emplace(std::string{"EMIT"}, osEmit);

    auto osCR = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {cr(stack, returnStack, dictionary, os);};
    this->emplace(std::string{"CR"}, osCR);

    auto osDotS = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dotS(stack, returnStack, dictionary, os);};
    this->emplace(std::string{".S"}, osDotS);

    this->emplace(std::string{"`"}, &addressOf);

    this->emplace(std::string{"DUMPDICT"}, [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dumpDictionary(stack, returnStack, dictionary, os);});

    this->emplace(std::string{"DROP"}, &drop);
    this->emplace(std::string{"INCLUDE"}, &include);
}

void addressOf(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    const auto funcName = stackToString(stack, returnStack, dictionary);
    const auto& dictionaryIt = dictionary.find(funcName);
    if (dictionaryIt == dictionary.end())
    {
        throw std::runtime_error("Tried to take address of unknown function " + funcName);
    }
    stack.pop_back(); // Could potentially use DROP
    const auto func = dictionaryIt->second.target<FthFunc>();
    const auto funcAsInt = reinterpret_cast<uintptr_t>(func);
    stack.push_back(funcAsInt);
}

void wordOf(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{

}

