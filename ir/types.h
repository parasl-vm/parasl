#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <cstdint>

using ImmType = uint64_t;
using IdType = std::string;

enum class BasicTypes {
    INVALID = 0,
    INT, // int32_t
    DOUBLE, // double
};

struct VRegType
{
    BasicTypes type;
};


#endif  // TYPES_H