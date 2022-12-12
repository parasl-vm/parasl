#include "inst.h"
#include "function.h"

void Inst::Dump()
{
    std::cout << inst_id_ << ":" << owner_->GetName() << " ";
    DumpOpcode();
    std::cout << " ";
    for (auto input: inputs_) {
        if (input->GetName() == NO_NAME)
            input->Dump();
        else {
            std::cout << input->GetName();
        }
        std::cout << " ";
    }
}