#ifndef INST_H
#define INST_H

#include <vector>

#include "opcodes.h"
#include "vreg.h"

class Function;

class Inst
{
  public:
    // maybe just a vector of VReg* as input?
    template <typename... input>
    static Inst* InstBuilder(Opcode opcode, input... inputs)
    {
        return new Inst(opcode, std::vector<VReg*>{ inputs... });
    }

    ACCESSOR_MUTATOR(opcode_, Opcode, Opcode);
    ACCESSOR_MUTATOR(inst_id_, InstId, uint32_t);
    ACCESSOR_MUTATOR(owner_, Function, Function*);

    VReg* GetInput(uint32_t index) const
    {
        assert(index < inputs_.size());
        return inputs_[index];
    }

    uint32_t GetInputsSize() const
    {
        return inputs_.size();
    }

    const std::vector<VReg*>& GetInputs() const
    {
        return inputs_;
    }

    void Dump();

  private:
    Inst(Opcode opcode, std::vector<VReg*> inputs) : opcode_(opcode), inputs_(inputs)
    {
        ++max_inst_id_;
        inst_id_ = max_inst_id_;
    }

    void DumpOpcode()
    {
#define PRINT_OPCODE(name, ...)                                                                   \
    case Opcode::name:                                                                            \
        std::cout << #name;                                                                       \
        return;

        switch (opcode_) {
            OPCODES_LIST(PRINT_OPCODE)
        default:
            assert(0);
#undef PRINT_OPCODE
        }
    }

    Opcode opcode_;
    std::vector<VReg*> inputs_;
    uint32_t inst_id_ = 0;
    Function* owner_ = nullptr;
    static inline uint32_t max_inst_id_ = 0;
};

#endif // INST_H