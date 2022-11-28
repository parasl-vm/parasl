#define OPCODES_LIST(_)                                                                                           \
    _(INTRINSIC_INPUT /* input stream:int dst:vreg */) \
    _(INTRINSIC_PRINT /* print src:vreg */) \
    _(MOV /* mov src:vreg dst:vreg */) \
    _(FINALIZE_VM) \
