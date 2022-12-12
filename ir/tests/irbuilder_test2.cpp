#include "gtest/gtest.h"

#include "ir/program.h"

// #define INST Inst::InstBuilder // linter warnings
#define VREG VReg::VRegBuilder

TEST(IR_BUILDER_TEST_2, TEST_1)
{
    // ================================================
    // var_1_ = input(0);
    // Arr1 = input(0..2) : int[3];
    // _arr_2: int[2]  = input(0..1);

    // struct1 = glue(var_1_, Arr1);
    // output(struct1[0]);

    // struct2 = glue(var_1_ : x, Arr1 : y, _arr_2 : z);
    // struct2[1][0] = -1;
    // struct2.z[1] = -2;
    // output(struct2.x);
    // ================================================


    // Use different vregs, v - int/undef, a - arr, s - struct/object

    // ------------------------------
    // CALL_INTRINSIC_INPUT, v_tmp
    // MOV v0, v_tmp
    // ------------------------------
    // ARRAY a0, int, 3
    // CALL_INTRINSIC_INPUT, v_tmp          // If var len of array possible use jump to fill it
    // MOV a0[0], v_tmp
    // CALL_INTRINSIC_INPUT, v_tmp
    // MOV a0[1], v_tmp
    // CALL_INTRINSIC_INPUT, v_tmp
    // MOV a0[2], v_tmp
    // ------------------------------
    // ARRAY a1, int, 2
    // CALL_INTRINSIC_INPUT, v_tmp
    // MOV a1[0], v_tmp
    // CALL_INTRINSIC_INPUT, v_tmp
    // MOV a1[1], v_tmp
    // ------------------------------
    // STRUCT s0
    // OBJADDF s0, undef, ""              // s0 - obj register, undef - type of var_1_, 0 - name for field
    // OBJADDF s0, arr, ""
    // ARRAY s0.1, int, 3
    // MOV s0.0, v_tmp
    // MOV s0.1, a0                       // Maybe better to use array copy, or loop, or move will be general
    // ------------------------------
    // CALL_INTRINSIC_OUTPUT 0, s0.0
    // ------------------------------
    // STRUCT s1
    // OBJADDF s1, undef, "x"
    // OBJADDF s1, arr, "y"
    // OBJADDF s1, arr, "z"
    // ARRAY s1.1, int, 3
    // ARRAY s1.2, int, 2
    // MOV s1.0, v_tmp
    // MOV s1.1, a0    
    // MOV s1.2, a1
    // -----------------------------
    // MOVI s1.1[0], -1
    // -----------------------------
    // MOVI s1.2[1], -2
    // -----------------------------
    // CALL_INTRINSIC_OUTPUT 0, s1.0
}