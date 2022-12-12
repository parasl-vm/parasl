#ifndef OPCODES_H
#define OPCODES_H

// maybe it is not needed
#define TYPE_LIST(_)                                    \
    _(InstWithTwoInputs)                                \
    _(InstWithOneInput)                                 \
    _(InstWithRangeInputs)

#define OPCODES_LIST(_)                                     \
    _(CALL_INTRINSIC_INPUT /* input stream:int dst:vreg */)    \
    _(CALL_INTRINSIC_OUTPUT /* output stream:int src:vreg */)  \
    _(MOV, InstWithTwoInputs /* mov dst:vreg src:vreg */) \
    _(ADD, /* mov dst:vreg src1:vreg src2:vreg*/)   \
    _(SUB, /* mov dst:vreg src1:vreg src2:vreg*/)   \
    _(MUL, /* mov dst:vreg src1:vreg src2:vreg*/)   \
    _(DIV, /* mov dst:vreg src1:vreg src2:vreg*/)   \
    _(FINALIZE_VM)


enum class Opcode {
#define INST_TYPES(op, ...) op,
    OPCODES_LIST(INST_TYPES)
#undef INST_TYPES
    INVALID
};

enum class OpcodeType {
#define INST_TYPES(op, ...) op,
    TYPE_LIST(INST_TYPES)
#undef INST_TYPES
    INVALID
};

#endif // OPCODES_H
