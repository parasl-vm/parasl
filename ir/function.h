#ifndef FUNCTION_H
#define FUNCTION_H

#include "inst.h"

class Function {
public:
    std::vector<Inst> insts;
    std::vector<VRegType> vregs;
};

#endif  // FUNCTION_H