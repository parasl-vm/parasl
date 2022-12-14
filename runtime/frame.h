#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include "ir/function.h"
#include "types.h"

#include <vector>

struct Frame {
    Frame(Function* f) : function(f) {
        for (const auto& vreg : f->vregs) {
            local_variables.emplace_back(Value(vreg.type));
        }
    }

    unsigned int ip = 0;
    Function* function;

    std::vector<Value> local_variables;
};

#endif  // FRAME_H_INCLUDED
