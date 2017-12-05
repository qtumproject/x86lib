#include "x86test.h"

TEST_CASE("mov_eax_imm32", "[mov]") {
    x86Tester test;
    x86Checkpoint check = test.LoadCheckpoint();
    test.Run("mov eax, 0x12345678");
    check.SetReg32(EAX, 0x12345678);
    test.Compare(check);
}

TEST_CASE("mov_ax_imm16", "[mov]") {
    x86Tester test;
    x86Checkpoint check = test.LoadCheckpoint();
    test.Run("mov ax, 0x1234");
    check.SetReg16(AX, 0x1234);
    test.Compare(check);
}

TEST_CASE("modrm ebp+eax*2", "[modrm]") {
    x86Tester test;
    test.Run(
"mov ebp, _tmp\n"
"mov eax, 4\n"
"mov ebx, [ebp + eax * 2]\n" //resolves to _tmp + 4 * 2
"jmp _end\n"
"_tmp: dw 0, 0, 0, 0\n"
"dd 0x12345678\n");
    x86Checkpoint check = test.LoadCheckpoint();
    //load checkpoint after running so that we can just set the registers we care about the results of
    check.SetReg32(EBX, 0x12345678);
    check.SetReg32(EAX, 4);
    test.Compare(check);
}
TEST_CASE("modrm ebp+disp*scale", "[modrm]") {
    x86Tester test;
    test.Run(
"mov ebp, _tmp\n"
"mov ebx, [ebp + 4 * 2]\n" //resolves to _tmp + 4 * 2
"jmp _end\n"
"_tmp: dw 0, 0, 0, 0\n"
"dd 0x12345678\n");
    x86Checkpoint check = test.LoadCheckpoint();
    //load checkpoint after running so that we can just set the registers we care about the results of
    check.SetReg32(EBX, 0x12345678);
    test.Compare(check);
}

TEST_CASE("modrm16 si+di+disp", "[modrm]") {
    x86Tester test;
    test.Run(
"mov bx, _tmp\n"
"mov si, 6\n"
"mov ebx, [bx + si + 2]\n" //resolves to _tmp + 6 + 2
"jmp _end\n"
"_tmp: dw 0, 0, 0, 0\n"
"dd 0x12345678\n");
    x86Checkpoint check = test.LoadCheckpoint();
    //load checkpoint after running so that we can just set the registers we care about the results of
    check.SetReg32(EBX, 0x12345678);
    test.Compare(check);
}


