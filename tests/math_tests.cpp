#include "catch.hpp"

#define private public
#define protected public
#include "x86lib.h"
#include "x86lib_internal.h"
#include "x86test.h"

using namespace x86Lib;
using namespace std;

//operation tests

TEST_CASE("Add", "[math ops]"){
    x86CPU cpu;
    cpu.Add8(155, 101); //signed: -101 + 101 = 0
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.af == 1); // B + 5 = 0x10
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.of == 0);

}


//opcode tests.. 
TEST_CASE("add_eax_imm32", "[add]") {
    x86Tester test;
    x86Checkpoint check = test.LoadCheckpoint();
    test.Run(
"mov eax, 100\n"
"add eax, -100\n");
    check.SetReg32(EAX, 0);
    check.SetOF();
    check.UnsetSF();
    check.SetZF();
    test.Compare(check);
}




