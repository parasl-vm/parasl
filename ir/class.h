#ifndef CLASS_H
#define CLASS_H

#include <vector>

#include "function.h"

class Class {
public:

private:
    std::unordered_map<std::string, Function*> methods_;
    std::unordered_map<std::string, VReg*> fields_;
};

#endif  // CLASS_H