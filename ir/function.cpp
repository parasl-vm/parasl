#include "function.h"
#include "layer.h"

void Function::Dump()
{
    std::cout << "Function \"" << name_ << "\"\nOwner: ";
    if (GetClass() != nullptr) {
        // std::cout << "Class " << GetClass()->GetName();
    } else if (GetLayer() != nullptr) {
        std::cout << "Layer \"" << GetLayer()->GetName() << "\"\n";
    }
    std::cout << "Local vars [";
    bool pretty_flag = false;
    for (auto loc_var: local_vars_) {
        pretty_flag = true;
        std::cout << "\n\t";
        loc_var.second->Dump();
        std::cout << ",";
    }
    if (pretty_flag)
        std::cout << "\n";
    std::cout << "]\n\n";
    for (auto inst: insts_) {
        inst->Dump();
        std::cout << "\n";
    }
    std::cout << "\n";
}