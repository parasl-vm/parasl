#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <unordered_map>

#include "class.h"
#include "function.h"

struct Layer {
    std::unordered_map<std::string, Class> class_table;
    std::unordered_map<std::string, Function> function_table; // at least contains main
};

#endif  // LAYER_H