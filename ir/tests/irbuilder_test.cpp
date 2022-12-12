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
    // output(0, v_1);
    // else {
    //     output(0, 44);
    //     output(0, 55);
    // }
    // -----------------------------------------------------------
    // MOV __var__ 6
    // MOV v_1 44.55
}

TEST(IR_BUILDER_TEST, TEST_4)
{
    // if (__var__ <= 0)
    // output(0, v0);
    // else
    //     output(0, 88);

    // output(0, 99);
    // -----------------------------------------------------------
}

TEST(IR_BUILDER_TEST, TEST_5)
{
    // if (__var__ <= 9)
    // if (__var__ <= 8)
    //     __var__ = 12;
    // else
    //     __var__ = 11;
    // -----------------------------------------------------------
}

TEST(IR_BUILDER_TEST, TEST_6)
{
    // for (x in 0:5) {
    // for (y in 0:8:2)
    //     output(0, x + y);

    // x = 1 < v0 <= 89;
    // output(0, 2 * x);

    // while (0)
    //     while (1)
    //         output(0, x - y);
    // }
    // -----------------------------------------------------------
}