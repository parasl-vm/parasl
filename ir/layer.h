#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <unordered_map>

#include "class.h"
#include "function.h"

class Layer {
public:
    Layer(std::string name) : name_(name)
    {
        CreateFunction("main");
    }

    template<typename Type>
    void CreateGlobalVariable(Type value, std::string name)
    {
        assert(global_vars_.find(name) == global_vars_.end());
        global_vars_[name] = VReg::VRegBuilder(value);
        global_vars_[name]->SetLayer(this);
        global_vars_[name]->SetName(name);
    }

    void CreateGlobalVariableEmpry(std::string name)
    {
        assert(global_vars_.find(name) == global_vars_.end());
        global_vars_[name] = VReg::VRegBuilderEmpty();
        global_vars_[name]->SetLayer(this);
        global_vars_[name]->SetName(name);
    }

    void SetEmptyGlobalVariable(std::string name, VReg *new_vreg)
    {
        assert(global_vars_.find(name) != global_vars_.end());
        assert(global_vars_[name]->GetType() == VRegType::UNINITIALIZED);
        global_vars_[name] = new_vreg;
    }

    void CreateFunction(std::string name)
    {
        assert(function_table_.find(name) == function_table_.end());
        function_table_[name] = new Function(name);
        function_table_[name]->SetLayer(this);
    }

    Function* GetFunction(std::string name)
    {
        if (function_table_.find(name) != function_table_.end())
            return function_table_[name];
        return nullptr;
    }

    void Dump()
    {
        std::cout << "Layer \"" << name_ << "\"\n";
        std::cout << "Global vars [";
        for (auto gl_var: global_vars_) {
            std::cout << "\n";
            gl_var.second->Dump();
            std::cout << ",";
        }
        std::cout << "]\n\n";

        // TODO(itrubachev) add dump of classes

        for (auto func: function_table_) {
            func.second->Dump();
            std::cout << "\n";
        }
    }

    ACCESSOR_MUTATOR(name_, Name, std::string)

private:
    std::unordered_map<std::string, Class*> class_table_;
    std::unordered_map<std::string, Function*> function_table_; // at least contains main
    std::unordered_map<std::string, VReg*> global_vars_;

    uint8_t id_ = 0;
    std::string name_;
};

#endif  // LAYER_H
