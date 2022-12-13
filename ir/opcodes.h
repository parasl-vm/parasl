#ifndef OPCODES_H
#define OPCODES_H

#define OPCODES_LIST(_)                                                                           \
    _(CALL_INTRINSIC_INPUT /* input stream:int dst:vreg */)                                       \
    _(CALL_INTRINSIC_OUTPUT /* output stream:int src:vreg */)                                     \
    _(MOV /* dst:vreg src:vreg */)                                                                \
    _(ADD /* dst:vreg src1:vreg src2:vreg */)                                                     \
    _(SUB /* dst:vreg src1:vreg src2:vreg */)                                                     \
    _(MUL /* dst:vreg src1:vreg src2:vreg */)                                                     \
    _(DIV /* dst:vreg src1:vreg src2:vreg */)                                                     \
    _(JEQ /* dst_inst_index:vreg src1:vreg == src2:vreg */)                                       \
    _(JNE /* dst_inst_index:vreg src1:vreg != src2:vreg */)                                       \
    _(JLT /* dst_inst_index:vreg src1:vreg < src2:vreg */)                                        \
    _(JLE /* dst_inst_index:vreg src1:vreg <= src2:vreg */)                                       \
    _(JGT /* dst_inst_index:vreg src1:vreg > src2:vreg */)                                        \
    _(JGE /* dst_inst_index:vreg src1:vreg >= src2:vreg */)                                       \
    _(JMP /* dst_inst_index:vreg */)                                                              \
    _(FINALIZE_VM)

enum class Opcode
{
#define INST_TYPES(op, ...) op,
    OPCODES_LIST(INST_TYPES)
#undef INST_TYPES
        NUM_OPCODES
};

#endif // OPCODES_H
