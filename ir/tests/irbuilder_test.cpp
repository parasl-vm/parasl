#include "gtest/gtest.h"

#include "ir/program.h"

// #define INST Inst::InstBuilder // linter warnings
#define VREG VReg::VRegBuilder


TEST(IR_BUILDER_TEST, TEST_1)
{
    // layer(0, "square") {
    //   v0 : int = input(0);
    //   output(0, v0);
    // }
    // -----------------------------------------------------------
    // CALL_INTRINSIC_INPUT 0, v_tmp // compute right expression
    // MOV v0, v_tmp // assign computed expression to result variable
    // CALL_INTRINSIC_OUTPUT 0, v0

    Program p;
    p.AddLayer("square");
    Layer *l0 = p.GetZeroLayer();
    Function *main = l0->GetFunction("main");
    VReg* vreg_tmp = VREG(VRegType::UNINITIALIZED); // not necessary
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_INPUT, VREG(0),
                  vreg_tmp /* or VREG_EMPTY("v_tmp") */));
    main->AddInst(Inst::InstBuilder(Opcode::MOV, VREG(VRegType::INT, "v0"),
                  vreg_tmp /* or main->GetLocalVariable("v_tmp") */));
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_OUTPUT, VREG(0), main->GetLocalVariable("v0")));

    l0->Dump();
}

TEST(IR_BUILDER_TEST, TEST_2)
{
    // v_1 = 3.5 + 7 * 5.75 + (1 + 0.6) / 2;
    // ___var___: int(21) = (1 + 5);
    // -----------------------------------------------------------
    // ADD tmp0 1 0.6
    // DIV tmp1 tmp0 2
    // MUL tmp2 7 5.75
    // ADD tmp3 tmp2 tmp1
    // ADD tmp4 tmp3 3.5
    // MOV v_1 tmp4
    // ADD tmp5 1 5
    // MOV __var__ tmp5

    Program p;
    p.AddLayer("test2");
    Layer *l0 = p.GetZeroLayer();
    Function *main = l0->GetFunction("main");
    std::vector<VReg*> tmp_vregs;
    for (int i = 0; i < 6; ++i) {
        tmp_vregs.push_back(VREG(VRegType::UNINITIALIZED));
    }

    main->AddInst(Inst::InstBuilder(Opcode::ADD, tmp_vregs[0], VREG(1), VREG(0.6)));
    main->AddInst(Inst::InstBuilder(Opcode::DIV, tmp_vregs[1], tmp_vregs[0], VREG(2)));
    main->AddInst(Inst::InstBuilder(Opcode::MUL, tmp_vregs[2], VREG(7), VREG(5.75)));
    main->AddInst(Inst::InstBuilder(Opcode::ADD, tmp_vregs[3], tmp_vregs[2], tmp_vregs[1]));
    main->AddInst(Inst::InstBuilder(Opcode::ADD, tmp_vregs[4], tmp_vregs[3], VREG(3.5)));
    main->AddInst(Inst::InstBuilder(Opcode::MOV, VREG(VRegType::UNINITIALIZED, "v_1"), tmp_vregs[4]));
    main->AddInst(Inst::InstBuilder(Opcode::ADD, tmp_vregs[5], VREG(1), VREG(5)));
    main->AddInst(Inst::InstBuilder(Opcode::MOV, VREG(VRegType::CUSTOM_INT, "__var__", 21), tmp_vregs[5]));

    l0->Dump();
}

TEST(IR_BUILDER_TEST, TEST_3)
{
    // __var__ = 6
    // v_1 = 44.55
    // if (__var__ <= 6)
    //     output(0, v_1);
    // else {
    //     output(0, 44);
    //     output(0, 55);
    // }
    // -----------------------------------------------------------
    // 0. MOV __var__ 6
    // 1. MOV v_1 44.55
    // 2. JLE 6 __var__ 6
    //    False branch
    // 3. CALL_INTRINSIC_OUTPUT 0 44
    // 4. CALL_INTRINSIC_OUTPUT 0 55
    // 5. JMP 7
    //    True branch
    // 6. CALL_INTRINSIC_OUTPUT 0 v_1
    // 7. ...

    Program p;
    p.AddLayer("test3");
    Layer *l0 = p.GetZeroLayer();
    Function *main = l0->GetFunction("main");

    main->AddInst(Inst::InstBuilder(Opcode::MOV, VREG(VRegType::UNINITIALIZED, "__var__"), VREG(6)));
    main->AddInst(Inst::InstBuilder(Opcode::MOV, VREG(VRegType::UNINITIALIZED, "v_1"), VREG(44.55)));
    main->AddInst(Inst::InstBuilder(Opcode::JLE, VREG(6), main->GetLocalVariable("__var__"), VREG(6)));
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_OUTPUT, VREG(0), VREG(44)));
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_OUTPUT, VREG(0), VREG(45)));
    main->AddInst(Inst::InstBuilder(Opcode::JMP, VREG(7)));
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_OUTPUT, VREG(0), main->GetLocalVariable("v_1")));

    l0->Dump();
}

TEST(IR_BUILDER_TEST, TEST_4)
{
    // __var__ = 6
    // if (__var__ <= 9)
    //     if (__var__ <= 8)
    //         __var__ = 12;
    //     else
    //         __var__ = 11;
    // -----------------------------------------------------------
    // 0. MOV __var__ 6
    // 1. JLE 2 __var__ 9
    //    False branch 
    // 2. JMP 7
    //    True branch
    // 3.     JLE 6 __var__ 8
    //        False branch
    // 4.     MOV __var__ 11
    // 5.     JMP 7
    //        True branch
    // 6.     MOV __var__ 12
    // 7. ...

    Program p;
    p.AddLayer("test4");
    Layer *l0 = p.GetZeroLayer();
    Function *main = l0->GetFunction("main");

    main->AddInst(Inst::InstBuilder(Opcode::MOV, VREG(VRegType::UNINITIALIZED, "__var__"), VREG(6)));
    main->AddInst(Inst::InstBuilder(Opcode::JLE, VREG(2), main->GetLocalVariable("__var__"), VREG(9)));
    main->AddInst(Inst::InstBuilder(Opcode::JMP, VREG(7)));
    main->AddInst(Inst::InstBuilder(Opcode::JLE, VREG(6), main->GetLocalVariable("__var__"), VREG(8)));
    main->AddInst(Inst::InstBuilder(Opcode::MOV, main->GetLocalVariable("__var__"), VREG(11)));
    main->AddInst(Inst::InstBuilder(Opcode::JMP, VREG(7)));
    main->AddInst(Inst::InstBuilder(Opcode::MOV, main->GetLocalVariable("__var__"), VREG(12)));

    l0->Dump();
}

TEST(IR_BUILDER_TEST, TEST_5)
{
    // for (x in 0:5) {
    //     for (y in 0:8:2)
    //         output(0, x + y);
    // 
    //     x = 1 < v0 <= 89; // what does this mean? Skipping
    //     output(0, 2 * x);
    // 
    //     while (0 == 1)
    //         while (1 == 1)
    //             output(0, x - y);
    // }
    // -----------------------------------------------------------
    //    Loop header
    // 0. MOV x 0
    // 1. JLT 4 x 5
    // 2. ADD x x 1
    // 3. JLT 4 x 5
    //    Loop body
    //        Loop header
    // 4.     MOV y 0
    // 5.     JLT 8 y 8
    // 6.     ADD y y 2
    // 7.     JLT 8 y 8
    // 8.     Loop body
    // 9.         ADD tmp1 x y   
    // 9.         CALL_INTRINSIC_OUTPUT 0 tmp1
    // 10.        JMP 6
    // 11.    MUL tmp2 2 x
    // 12.    CALL_INTRINSIC_OUTPUT 0 tmp2
    // 13.    Loop header
    // 14.    MOV tmp3 0
    // 15.    JEQ tmp3 1
    //            Loop body
    //            ...
    //            JMP 15
    // ...
    // JMP 2
}

