#include "dictionary.hpp"
#include "io.hpp"
#include "math.hpp"
#include "stack_funcs.hpp"
#include "std_string.hpp"
#include "fth_string.hpp"
#include <stdexcept>


void Dictionary::populateDictionary(std::ostream& os)
{
    this->emplace(std::string{"NUMBER"}, &number);

    this->emplace(std::string{"+"}, &add);
    this->emplace(std::string{"."}, [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dot(stack, returnStack, dictionary, os);});

    auto osEmit = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {emit(stack, returnStack, dictionary, os);};
    this->emplace(std::string{"EMIT"}, osEmit);

    auto osCR = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {cr(stack, returnStack, dictionary, os);};
    this->emplace(std::string{"CR"}, osCR);

    auto osDotS = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dotS(stack, returnStack, dictionary, os);};
    this->emplace(std::string{".S"}, osDotS);

    this->emplace(std::string{"`"}, &addressOf);
    this->emplace(std::string{"TICK"}, &addressOf);
    this->emplace(std::string{"TOCK"}, &wordOf);  // TODO what is this supposed to be called?


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
    //const auto func = dictionaryIt->second.target<FthFunc>();
    const auto func = &(dictionaryIt->second);
    const auto funcAsInteger = reinterpret_cast<uintptr_t>(func);
    
    // On LP64 a pointer takes up 2 ints.
    // TODO: consider having an Fth Pointer type that is similar to string.  So size(always 2), int1, int2.
    static_assert(std::is_same<Stack::value_type,int>::value);
    const int* funcAsIntPtr = reinterpret_cast<const int*>(&funcAsInteger);
    for (int index = 0; index != sizeof(funcAsInteger)/sizeof(int); ++index)
    {
        stack.push_back(funcAsIntPtr[index]);
    }
}

void wordOf(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)
{
    uintptr_t funcAsInteger{0};
    int* funcAsIntPtr = reinterpret_cast<int*>(&funcAsInteger); 
    for (int index = sizeof(funcAsInteger)/sizeof(int); index != 0; --index)
    {
        funcAsIntPtr[index-1] = stack.back();
        stack.pop_back();
    }

    auto it = std::find_if( std::begin(dictionary), std::end(dictionary),
          [&funcAsInteger]( const auto& dictElement )
          {
            return reinterpret_cast<uintptr_t>(&(dictElement.second)) == funcAsInteger; 
          } );
              
    if ( it == std::end(dictionary) ) 
    {
        throw std::runtime_error("Tried to find word of " + std::to_string(funcAsInteger));
    }
    stringToStack(stack, returnStack, dictionary, it->first);
}

