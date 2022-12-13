#ifndef RUNTIME_H_INCLUDED
#define RUNTIME_H_INCLUDED

#include "ir/program.h"

#define HANDLER_NAME(OPCODE) HandleOpcode##OPCODE
#define HANDLER_LABEL(OPCODE) do_##OPCODE
#define HANDLER_DECL(OPCODE) bool HANDLER_NAME(OPCODE)()
#define HANDLER_IMPL(OPCODE) bool Runtime::HANDLER_NAME(OPCODE)()

class Runtime
{
  public:
    Runtime(Program* p) : prog_(p)
    {
        pc_ = 0;
        cur_func_ = prog_->GetZeroLayer()->GetFunction("main");
    }

    bool InterpretationLoop()
    {
        static void* dtt[] = {
#define DISPATCH_ADRESSES(op, ...) &&HANDLER_LABEL(op),
            OPCODES_LIST(DISPATCH_ADRESSES)
            &&do_FINALIZE_VM
#undef DISPATCH_ADRESSES
        };
#define NEXT()                                                                                    \
    do {                                                                                          \
        cur_inst_ = cur_func_->GetInst(pc_++);                                                    \
        goto* dtt[static_cast<size_t>(cur_inst_->GetOpcode())];                                   \
    } while (0)

        NEXT();

#define DISPATCH_HANDLERS(op, ...)                                                                \
    HANDLER_LABEL(op) : HANDLER_NAME(op)();                                                       \
    NEXT();

        OPCODES_LIST(DISPATCH_HANDLERS)
#undef DISPATCH_HANDLERS
    do_FINALIZE_VM:
        return true;
    }

#define DECLARE_HANDLERS(op, ...) HANDLER_DECL(op);
    OPCODES_LIST(DECLARE_HANDLERS)
#undef DECLARE_HANDLERS

  private:
    uint32_t pc_ = 0;
    Inst* cur_inst_;
    Function* cur_func_;
    Program* prog_;
};

#endif