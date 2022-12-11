#include "layer.h"

// square.pcl
// layer(0, "square") {
//   v0 : int = input(0);
//   output(0, v0);
// }

// square.0.pcl
// v0 : int = input(0);
// output(0, v0);

// equivalent bytecode
// CALL_INTRINSIC_INPUT 0, v_tmp // compute right expression
// MOV v0, v_tmp // assign computed expression to result variable
// CALL_INTRINSIC_OUTPUT 0, v0

// #define INST Inst::InstBuilder
#define VREG VReg::VRegBuilder
#define VREG_EMPTY VReg::VRegBuilderEmpty

int main()
{
    Layer l0("square");
    Function *main = l0.GetFunction("main");
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_INPUT, VREG(0), VREG_EMPTY("v_tmp")));
    main->AddInst(Inst::InstBuilder(Opcode::MOV, VREG_EMPTY("v0"), main->GetLocalVariable("v_tmp")));
    main->AddInst(Inst::InstBuilder(Opcode::CALL_INTRINSIC_OUTPUT, VREG(0), main->GetLocalVariable("v0")));

    l0.Dump();

    return 0;
}