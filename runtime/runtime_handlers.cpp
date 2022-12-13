#include "runtime.h"
#include <cassert>
#include <iostream>

// CALL_INTRINSIC_INPUT
HANDLER_IMPL(CALL_INTRINSIC_INPUT)
{
    assert(cur_inst_->GetOpcode() == Opcode::CALL_INTRINSIC_INPUT);
    assert(cur_inst_->GetInputsSize() == 2);
    assert(cur_inst_->GetInput(0)->IsInt());
    assert(cur_inst_->GetInput(1)->IsInitialized());

    switch (cur_inst_->GetInput(1)->GetType()) {
    case VRegType::CUSTOM_INT: {
        int64_t res = 0;
        std::cin >> res;
        cur_inst_->GetInput(1)->SetCustomInt(res);
        break;
    }
    case VRegType::INT: {
        int64_t res = 0;
        std::cin >> res;
        cur_inst_->GetInput(1)->SetInt(res);
        break;
    }
    case VRegType::DOUBLE: {
        double res = 0;
        std::cin >> res;
        cur_inst_->GetInput(1)->SetDouble(res);
        break;
    }
    default:
        UNREACHABLE("Unsupported type in input");
    }

    return true;
}

// CALL_INTRINSIC_OUTPUT
HANDLER_IMPL(CALL_INTRINSIC_OUTPUT)
{
    assert(cur_inst_->GetOpcode() == Opcode::CALL_INTRINSIC_OUTPUT);
    assert(cur_inst_->GetInputsSize() == 2);
    assert(cur_inst_->GetInput(0)->IsInt());
    assert(cur_inst_->GetInput(1)->IsInitialized());

    switch (cur_inst_->GetInput(1)->GetType()) {
    case VRegType::CUSTOM_INT: {
        std::cout << cur_inst_->GetInput(1)->GetCustomInt();
        break;
    }
    case VRegType::INT: {
        std::cout << cur_inst_->GetInput(1)->GetInt();
        break;
    }
    case VRegType::DOUBLE: {
        std::cout << cur_inst_->GetInput(1)->GetDouble();
        break;
    }
    default:
        UNREACHABLE("Unsupported type in output");
    }

    return true;
}

// MOV
HANDLER_IMPL(MOV)
{
    assert(cur_inst_->GetOpcode() == Opcode::MOV);
    assert(cur_inst_->GetInputsSize() == 2);
    assert(cur_inst_->GetInput(1)->IsInitialized());

    if (cur_inst_->GetInput(0)->IsInitialized()) {
        assert(cur_inst_->GetInput(0)->GetType() == cur_inst_->GetInput(1)->GetType());
    } else {
        cur_inst_->GetInput(0)->SetType(cur_inst_->GetInput(1)->GetType());
    }

    switch (cur_inst_->GetInput(1)->GetType()) {
    case VRegType::CUSTOM_INT: {
        cur_inst_->GetInput(0)->SetCustomInt(cur_inst_->GetInput(1)->GetCustomInt());
        break;
    }
    case VRegType::INT: {
        cur_inst_->GetInput(0)->SetInt(cur_inst_->GetInput(1)->GetInt());
        break;
    }
    case VRegType::DOUBLE: {
        cur_inst_->GetInput(0)->SetDouble(cur_inst_->GetInput(1)->GetDouble());
        break;
    }
    case VRegType::OBJECT_TYPE: {
        // cur_inst_->GetInput(0)->SetPointer(cur_inst_->GetInput(1)->GetPointer());
        break;
    }
    default:
        UNREACHABLE("Unsupported type in input");
    }

    return true;
}

// static void CheckArithInstr() {
    
// }

// ADD
HANDLER_IMPL(ADD)
{
    assert(cur_inst_->GetOpcode() == Opcode::CALL_INTRINSIC_OUTPUT);
    assert(cur_inst_->GetInputsSize() == 3);
    
    auto input_0 = cur_inst_->GetInput(0);
    auto input_1 = cur_inst_->GetInput(1);
    auto out = cur_inst_->GetInput(2);

    assert(input_0->IsInt() || input_0->IsDouble());
    assert(input_1->IsInt() || input_1->IsDouble());

    if (out->IsInitialized())  {
        // use it
    } else {
        // int a = 0;
    }
    
    switch (cur_inst_->GetInput(1)->GetType()) {
    case VRegType::CUSTOM_INT: {
        std::cout << cur_inst_->GetInput(1)->GetCustomInt();
        break;
    }
    case VRegType::INT: {
        std::cout << cur_inst_->GetInput(1)->GetInt();
        break;
    }
    case VRegType::DOUBLE: {
        std::cout << cur_inst_->GetInput(1)->GetDouble();
        break;
    }
    default:
        UNREACHABLE("Unsupported type in output");
    }

    return true;
}

// SUB
HANDLER_IMPL(SUB)
{
    return true;
}

// MUL
HANDLER_IMPL(MUL)
{
    return true;
}

// DIV
HANDLER_IMPL(DIV)
{
    return true;
}

static void CheckJumpInstruction(Inst* inst)
{
    assert(inst->GetInputsSize() == 3);
    assert(inst->GetInput(0)->IsInt());
    assert(inst->GetInput(1)->IsInitialized());
    assert(inst->GetInput(2)->IsInitialized());
    assert(!inst->GetInput(1)->IsObj());
    assert(!inst->GetInput(2)->IsObj());
}

#define JMP_COMP_INST(comparator)   \
    CheckJumpInstruction(cur_inst_);  \
    double in1 = cur_inst_->GetInput(1)->GetInt() + \
                 cur_inst_->GetInput(1)->GetCustomInt() + cur_inst_->GetInput(1)->GetDouble();  \
    double in2 = cur_inst_->GetInput(2)->GetInt() + \
                 cur_inst_->GetInput(2)->GetCustomInt() + cur_inst_->GetInput(2)->GetDouble();  \
    if (in1 comparator in2) {   \
        uint8_t dst = cur_inst_->GetInput(0)->GetInt();  \
        pc_ = dst;  \
    }   

// JEQ
HANDLER_IMPL(JEQ)
{
    assert(cur_inst_->GetOpcode() == Opcode::JEQ);
    JMP_COMP_INST(==)    
    return true;
}

// JNE
HANDLER_IMPL(JNE)
{
    assert(cur_inst_->GetOpcode() == Opcode::JNE);
    JMP_COMP_INST(!=)
    return true;
}

// JLT
HANDLER_IMPL(JLT)
{
    assert(cur_inst_->GetOpcode() == Opcode::JLT);
    JMP_COMP_INST(<)
    return true;
}

// JLE
HANDLER_IMPL(JLE)
{
    assert(cur_inst_->GetOpcode() == Opcode::JLE);
    JMP_COMP_INST(<=)
    return true;
}

// JGT
HANDLER_IMPL(JGT)
{
    assert(cur_inst_->GetOpcode() == Opcode::JGT);
    JMP_COMP_INST(>)
    return true;
}

// JGE
HANDLER_IMPL(JGE)
{
    assert(cur_inst_->GetOpcode() == Opcode::JGE);
    JMP_COMP_INST(>=)
    return true;
}

// JMP
HANDLER_IMPL(JMP)
{
    assert(cur_inst_->GetOpcode() == Opcode::JMP);
    assert(cur_inst_->GetInputsSize() == 1);
    assert(cur_inst_->GetInput(0)->IsInt());
    uint8_t dst = cur_inst_->GetInput(0)->GetInt();
    pc_ = dst;
    return true;
}
