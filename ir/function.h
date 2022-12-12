#ifndef FUNCTION_H
#define FUNCTION_H

#include <unordered_map>
#include <string>
#include <variant>

#include "inst.h"

class Class;
class Layer;

class Function {
public:
    Function(std::string name) : name_(name) {}
    void AddInst(Inst* inst)
    {
        inst->SetFunction(this);
        for (auto input: inst->GetInputs()) {
            assert(input != nullptr);
            input->SetFunction(this);
            if (input->GetName() != NO_NAME) {
                local_vars_[input->GetName()] = input;
            }
        }
        insts_.push_back(inst);
    }

    const std::vector<Inst*>& GetInsts()
    {
        return insts_;
    }

    template <typename Type>
    void CreateLocalVariable(std::string name, Type value)
    {
        assert(local_vars_.find(name) == local_vars_.end());
        local_vars_[name] = VReg::VRegBuilder(value, name);
        local_vars_[name]->SetFunction(this);
    }

    VReg* GetLocalVariable(std::string name)
    {
        if (local_vars_.find(name) != local_vars_.end())
            return local_vars_[name];
        return nullptr;
    }

    void SetEmptyLocalVariable(std::string name, VReg *new_vreg)
    {
        assert(local_vars_.find(name) != local_vars_.end());
        assert(local_vars_[name]->GetType() == VRegType::UNINITIALIZED);
        local_vars_[name] = new_vreg;
    }

    // method in class
    Class* GetClass()
    {
        if (std::holds_alternative<Class*>(owner_))
            return std::get<Class*>(owner_);
        return nullptr;
    }

    // global function
    Layer* GetLayer()
    {
        if (std::holds_alternative<Layer*>(owner_))
            return std::get<Layer*>(owner_);
        return nullptr;
    }

    void SetClass(Class* class_)
    {
        owner_ = class_;
    }

    void SetLayer(Layer* layer)
    {
        owner_ = layer;
    }

    void Dump();

    ACCESSOR_MUTATOR(name_, Name, std::string)

private:
    std::vector<Inst*> insts_;
    std::unordered_map<std::string, VReg*> local_vars_;
    std::string name_ = "no name";
    std::variant<Class*, Layer*> owner_;
};

#endif  // FUNCTION_H
