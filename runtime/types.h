#ifndef RUNTIME_TYPES_H_INCLUDED
#define RUNTIME_TYPES_H_INCLUDED

#include "ir/types.h"
#include <cstdint>
#include <cassert>

struct Value {
    Value(const BasicTypes& t ) : type(t) {
        assert(t != BasicTypes::INVALID);
    }
    uint64_t value = 0;
    BasicTypes type;
};

#endif