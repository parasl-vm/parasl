#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>

#include "layer.h"

// now it is a useless class
class Program {
public:
    void AddLayer(std::string name)
    {
        layers_.push_back(new Layer(name));
    }

    Layer* GetZeroLayer()
    {
        return layers_[0];
    }

private:
    std::vector<Layer*> layers_;
};

#endif  // PROGRAM_H
