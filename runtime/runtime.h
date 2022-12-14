#ifndef RUNTIME_H_INCLUDED
#define RUNTIME_H_INCLUDED

#include "frame.h"
#include "ir/opcodes.h"
#include "ir/program.h"

class Runtime
{
  public:
    Runtime(const Program& p) : prog(p) {
        frames.emplace_back(Frame(prog.GetMain()));
        cur_frame = &(frames.back());
    }

    bool InterpretataionLoop()
    {
        static void* dtt[] = {
#define DISPATCH_ADRESSES(op, ...) &&op,
            OPCODES_LIST(DISPATCH_ADRESSES)
#undef DISPATCH_ADRESSES
        };
#define NEXT() goto* dtt[cur_frame->function->insts[cur_frame->ip++].opcode]

        NEXT();
#define DISPATCH_HANDLERS(op, ...)                                                                \
    op:                                                                                           \
    HandleOpcode##op();                                                                           \
    if constexpr (Inst::Opcode::##op != Inst::Opcode::FINALIZE_VM) { \
        NEXT(); \
    }
        OPCODES_LIST(DISPATCH_HANDLERS)
#undef DISPATCH_HANDLERS
    }

#define DECLARE_HANDLERS(op, ...) bool HandleOpcode##op();
    OPCODES_LIST(DECLARE_HANDLERS)
#undef DECLARE_HANDLERS

  private:
    Frame* cur_frame;
    std::vector<Frame> frames;

    Program prog;
};

#endif  // RUNTIME_H_INCLUDED
