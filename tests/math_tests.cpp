#include "catch.hpp"

#define private public
#define protected public
#include "x86lib.h"
#include "x86lib_internal.h"
#include "x86test.h"

using namespace x86Lib;
using namespace std;

//operation tests

//parity and SF flags is tested elsewhere, no need to retest here
TEST_CASE("Add", "[math ops]"){
    x86CPU cpu;
    REQUIRE(cpu.Add8(155, 101) == 0); //signed: -101 + 101 = 0
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.af == 1); // B + 5 = 0x10
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.of == 0);
    
    REQUIRE(cpu.Add16(100, 800) == 900);
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.af == 0); // 4 + 4 = 8
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.Add32(0xF00090FF, 0xF00121FA) == 0xE001B2F9); 
    //signed: -268,398,337 + -268,361,222 = -536,759,559
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.af == 1); // f + a = 0x19
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.Add32(0x7FFFFFFF, 0x7FFFFFFF) == 0xFFFFFFFE);
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.af == 1); // f + f = 0x1E
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1);
}
TEST_CASE("Sub", "[math ops]"){
    x86CPU cpu;
    REQUIRE(cpu.Sub8(155, 101) == 54); //signed: -101 - 101 = -202 = 54
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.af == 0); // B + 5 = 6
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1);
    
    REQUIRE(cpu.Sub16(100, 800) == (uint16_t)-700);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.af == 0); // 4 - 4 = 0
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.Sub32(0xF00090FF, 0xF00121FA) == 0xFFFF6F05); 
    //signed: -268,398,337 - -268,361,222 = -37115
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.af == 0); // f - a = 5
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.Sub32(0x7FFFFFFF, 0x7FFFFFFF) == 0); //-1 - -1 = 0
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.af == 0); // f -f = 0
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.Sub8(0xFA, 0xFF) == 0xFB); //signed: -6 - -1 = -5 = 0xFB
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.af == 0); // A - F = 0xFB
    REQUIRE(cpu.freg.bits.zf == 0);
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




