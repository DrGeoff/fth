#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

using Stack = std::vector<int>;
using ReturnStack = std::vector<int>;
using FthFunc = void(Stack& stack, ReturnStack& returnStack);
using Dictionary = std::unordered_map<std::string, std::function<FthFunc>>;

void process(std::istream& iss, Dictionary& dictionary, Stack& stack, ReturnStack& returnStack);
