#ifndef INST_H
#define INST_H

#include <vector>

#include "types.h"
#include "opcodes.h"

struct Inst
{
    enum Opcode {
#define INST_TYPES(op, ...) op,
    OPCODES_LIST(INST_TYPES)
#undef INST_TYPES
        INVALID
    };

    Opcode opcode = Opcode::INVALID;

    std::vector<size_t> vregs;
    std::vector<IdType> ids;
    std::vector<ImmType> imms;
};

#endif // INST_H