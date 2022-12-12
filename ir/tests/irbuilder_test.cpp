#include "gtest/gtest.h"

#include "ir/program.h"

// #define INST Inst::InstBuilder // linter warnings
#define VREG VReg::VRegBuilder
#define VREG_EMPTY VReg::VRegBuilderEmpty


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
    VReg* vreg_tmp = VREG_EMPTY(); // not necessary
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_INPUT, VREG(0),
                  vreg_tmp /* or VREG_EMPTY("v_tmp") */));
    main->AddInst(Inst::InstBuilder(Opcode::MOV, VREG_EMPTY("v0"),
                  vreg_tmp /* or main->GetLocalVariable("v_tmp") */));
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_OUTPUT, VREG(0), main->GetLocalVariable("v0")));

    l0->Dump();
}

TEST(IR_BUILDER_TEST, TEST_2)
{
    // v_1 = 3.5 + 7 * 5.75 + (1 + 0.6) / 2;
    // ___var___: int(21) = (1 + 5);
    // -----------------------------------------------------------

    // l0->Dump();
}

TEST(IR_BUILDER_TEST, TEST_3)
{
    // if (__var__ <= 6)
    // output(0, v_1);
    // else {
    //     output(0, 44);
    //     output(0, 55);
    // }
    // -----------------------------------------------------------
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

TEST(IR_BUILDER_TEST, TEST_5)
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