#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>

#include "layer.h"

struct Program {
    Function* GetMain() {
        return &(layers[0].function_table.at("MAIN"));
    }

    std::vector<Layer> layers;
};

#endif  // PROGRAM_H