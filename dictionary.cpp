#include "dictionary.hpp"
#include "io.hpp"
#include "math.hpp"
#include "stack_funcs.hpp"


void Dictionary::populateDictionary(std::ostream& os)
{
    this->emplace(std::string{"+"}, &add);

    //auto osDot = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dot(stack, returnStack, dictionary, os);};
    this->emplace(std::string{"."}, [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dot(stack, returnStack, dictionary, os);});

    auto osEmit = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {emit(stack, returnStack, dictionary, os);};
    this->emplace(std::string{"EMIT"}, osEmit);

    auto osCR = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {cr(stack, returnStack, dictionary, os);};
    this->emplace(std::string{"CR"}, osCR);

    auto osDotS = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dotS(stack, returnStack, dictionary, os);};
    this->emplace(std::string{".S"}, osDotS);

    //auto osDumpDictionary = [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dumpDictionary(stack, returnStack, dictionary, os);};
    this->emplace(std::string{"DUMPDICT"}, [&os](Stack& stack, ReturnStack& returnStack, Dictionary& dictionary) mutable {dumpDictionary(stack, returnStack, dictionary, os);});

    this->emplace(std::string{"DROP"}, &drop);
    this->emplace(std::string{"INCLUDE"}, &include);
}

