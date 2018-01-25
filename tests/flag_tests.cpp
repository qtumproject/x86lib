#include "x86test.h"

TEST_CASE( "8-bit Add flags are correct", "[flags]" ) {
    x86CPU cpu;
    cpu.Add8(0, 0);
    REQUIRE((int) cpu.freg.bits.zf);
    REQUIRE((int) !cpu.freg.bits.of);
    REQUIRE((int) !cpu.freg.bits.cf);
}

TEST_CASE("asm test", "[test]") {
    x86Tester test;
    x86Checkpoint check = test.LoadCheckpoint();
    test.Run("mov eax, 0x12345678");
    check.SetReg32(EAX, 0x12345678);
    check.AdvanceEIP(5);
    test.Compare(check, true);
}

TEST_CASE("setne setnz", "[flags]") {
    x86Tester test;
    x86Checkpoint check = test.LoadCheckpoint();
    test.Run("mov eax, 1\n"
             "sub eax, 0\n"
             "setne cl\n"
             "setnz bl\n"
             "sete dl");
    check.UnsetZF();
    check.SetReg32(EAX, 1);
    check.SetReg8(CL, 1);
    check.SetReg8(BL, 1);
    check.SetReg8(DL, 0);
    test.Compare(check);
}

TEST_CASE("sete setz", "[flags]") {
    x86Tester test;
    x86Checkpoint check = test.LoadCheckpoint();
    test.Run("mov eax, 1\n"
             "sub eax, 1\n"
             "sete cl\n"
             "setz bl\n");
    REQUIRE(test.Check().Reg8(CL) == 1);
    REQUIRE(test.Check().Reg8(BL) == 1);
    check.SetZF();
    check.SetReg8(CL, 1);
    check.SetReg8(BL, 1);
}
