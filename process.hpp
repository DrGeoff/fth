#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

using Stack = std::vector<int>;
using ReturnStack = std::vector<int>;

// The purpose of some functions is to manipulate the dictionary so that must be available to all functions
//using Dictionary = std::unordered_map<std::string, std::function<void(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary)>>;
class Dictionary;
using FthFunc = void(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);

class Dictionary : public std::unordered_map<std::string, std::function<FthFunc>> 
{};

void process(std::istream& iss, Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);
void include(Stack& stack, ReturnStack& returnStack, Dictionary& dictionary);
