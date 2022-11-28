#include "runtime.h"
#include "ir/inst.h"
#include <iostream>
#include <cassert>

bool Runtime::HandleOpcodeINTRINSIC_INPUT() {
    auto inst = cur_frame->function->insts[cur_frame->ip];
    auto dst = &(cur_frame->local_variables[inst.vregs[0]]);
    switch (dst->type)
    {
    case BasicTypes::INT:
        int64_t res = 0;
        std::cin >> res;
        dst->value = static_cast<uint64_t>(res);
        break;
    case BasicTypes::DOUBLE:
        double res = 0;
        std::cin >> res;
        dst->value = static_cast<uint64_t>(res);
        break;
    case BasicTypes::INVALID:
    default:
        assert(0);
    }
}

bool Runtime::HandleOpcodeINTRINSIC_PRINT() {
    auto inst = cur_frame->function->insts[cur_frame->ip];
    auto src = cur_frame->local_variables[inst.vregs[0]];

    switch (src.type)
    {
    case BasicTypes::INT:
        std::cout << static_cast<int64_t>(src.value);
        break;
    case BasicTypes::DOUBLE:
        std::cout << static_cast<double>(src.value);
        break;
    case BasicTypes::INVALID:
    default:
        assert(0);
    }
}

bool Runtime::HandleOpcodeMOV() {
    auto inst = cur_frame->function->insts[cur_frame->ip];
    auto src = cur_frame->local_variables[inst.vregs[0]];
    auto dst = &(cur_frame->local_variables[inst.vregs[0]]);

    assert(src.type == dst->type);

    dst->value = src.value;
}