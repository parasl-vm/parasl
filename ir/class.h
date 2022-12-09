#ifndef CLASS_H
#define CLASS_H

#include <vector>

#include "field.h"
#include "function.h"

class Class {
public:

private:
    // or unordered_map, idk
    std::vector<Field> fields_;
    std::vector<Method> methods_;
};

#endif  // CLASS_H