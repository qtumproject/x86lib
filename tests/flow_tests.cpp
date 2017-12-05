#include "x86test.h"

TEST_CASE("op_jmp_relW", "[flow]"){
    x86Tester test;
    x86Checkpoint check = test.LoadCheckpoint();
    test.Assemble(
"jmp WORD test1\n" //0: 66 e9 xx xx
"nop\n" //4: 90
"test4:\n"
"nop\n" //5: 90
"test2:\n"
"jmp DWORD test4\n" //6: e9 xx xx xx xx
"nop\n" //B: 90
"nop\n" //C: 90
"test1:\n"
"jmp DWORD test3\n" //D: e9 xx xx xx xx
"test3:\n"
"nop\n" //12: 90
"jmp WORD test2\n" //13: 66 e9 xx xx
); 
    test.Run(1);
    check.SetEIP(0x1000 + 0x0D);
    test.Compare(check, true);
    test.Run(1);
    check.SetEIP(0x1000 + 0x12);
    test.Compare(check, true);
    test.Run(1);
    check.SetEIP(0x1000 + 0x13);
    test.Compare(check, true);
    test.Run(1);
    check.SetEIP(0x1000 + 0x06);
    test.Compare(check, true);
    test.Run(1);
    check.SetEIP(0x1000 + 0x05);
    test.Compare(check, true);
    test.Run(1);
    check.SetEIP(0x1000 + 0x06);
    test.Compare(check, true);
}
