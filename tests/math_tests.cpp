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
    REQUIRE(cpu.freg.bits.af == 1); // A - F = 0xFB
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.Sub8(0xFE, 0xFF) == 0xFF); //signed: -2 - -1 = -1
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.af == 1); // E - F
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.Sub8(-120, 50) == 0x56); //signed: -120 - 50 = -170
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.af == 0); // 8 - 2 = 6
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1);
}

/* All macro's evaluate to compile-time constants */

/* *** helper macros *** */

/* turn a numeric literal into a hex constant
(avoids problems with leading zeroes)
8-bit constants max value 0x11111111, always fits in unsigned long
*/
#define HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)

/* *** user macros *** */

/* for upto 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for upto 16-bit binary constants, MSB first */
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))

/* for upto 32-bit binary constants, MSB first */
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))

/* Sample usage:
B8(01010101) = 85
B16(10101010,01010101) = 43605
B32(10000000,11111111,10101010,01010101) = 2164238933
*/

/*
The OF flag is affected only on 1-bit shifts. For left shifts, the OF flag is set to 0 if the most significant bit
of the result is the same as the CF flag (that is, the top two bits of the original operand were the same);
otherwise, it is set to 1. For the SAR instruction, the OF flag is cleared for all 1-bit shifts. For the SHR
instruction, the OF flag is set to the most-significant bit of the original operand.
*/

TEST_CASE("ShiftLogicalLeft", "[math ops]"){
    x86CPU cpu;
    REQUIRE(cpu.ShiftLogicalLeft8(B8(01011100), 1) == B8(10111000));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1); //msb(result) ^ CF, 1 ^ 0

    REQUIRE(cpu.ShiftLogicalLeft8(0, 1) == 0); 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //msb(result) ^ CF, 0 ^ 0

    REQUIRE(cpu.ShiftLogicalLeft8(0xFA, 0) == 0xFA); //when shifting 0, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //msb(result) ^ CF, 0 ^ 0

    REQUIRE(cpu.ShiftLogicalLeft8(0xFA, 32) == 0xFA); //when shifting 32, all flags should be unmodified (only bottom 5 bits used)
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //msb(result) ^ CF, 0 ^ 0

    REQUIRE(cpu.ShiftLogicalLeft8(B8(11011100), 1) == B8(10111000));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //msb(result) ^ CF, 1 ^ 1

    REQUIRE(cpu.ShiftLogicalLeft16(B16(11011100, 00001100), 1) == B16(10111000, 00011000));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //msb(result) ^ CF, 1 ^ 1

    REQUIRE(cpu.ShiftLogicalLeft32(B32(11011100, 00001100, 00000000, 00001000), 1) == 
        B32(10111000, 00011000, 00000000, 00010000));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //msb(result) ^ CF, 1 ^ 1

    REQUIRE(cpu.ShiftLogicalLeft32(B32(01011100, 00001100, 00000000, 00001000), 1) == 
        B32(10111000, 00011000, 00000000, 00010000));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1); //msb(result) ^ CF, 1 ^ 1

    REQUIRE(cpu.ShiftLogicalLeft32(0xF00FABCD, 4) == 0x00FABCD0);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1);
}

TEST_CASE("ShiftLogicalRight", "[math ops]"){
    x86CPU cpu;
    REQUIRE(cpu.ShiftLogicalRight8(B8(11011101), 1) == B8(01101110));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 1);

    REQUIRE(cpu.ShiftLogicalRight8(0, 1) == 0); 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0); 

    REQUIRE(cpu.ShiftLogicalRight8(0xFA, 0) == 0xFA); //when shifting 0, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.ShiftLogicalRight8(0xFA, 32) == 0xFA); //when shifting 32, all flags should be unmodified (only bottom 5 bits used)
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0);


    REQUIRE(cpu.ShiftLogicalRight8(B8(11011100), 1) == B8(01101110));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1);
    
    REQUIRE(cpu.ShiftLogicalRight16(B16(11011100, 00001100), 1) == B16(01101110, 00000110));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1);

    REQUIRE(cpu.ShiftLogicalRight32(B32(11011100, 00001100, 00000000, 00001001), 1) == 
        B32(01101110, 00000110, 00000000, 00000100));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 1);
    REQUIRE(cpu.freg.bits.sf == 0);

    REQUIRE(cpu.ShiftLogicalRight32(0xF00FABCC, 4) == 0x0F00FABC);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //undefined, but x86lib sets it to 0
    REQUIRE(cpu.freg.bits.sf == 0);
}

TEST_CASE("ShiftArithmeticRight", "[math ops]"){
    x86CPU cpu;
    REQUIRE(cpu.ShiftArithmeticRight8(B8(11011101), 1) == B8(11101110));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.sf == 1);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.ShiftArithmeticRight8(B8(01011101), 1) == B8(00101110));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.ShiftArithmeticRight8(0, 1) == 0); 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0); 

    REQUIRE(cpu.ShiftArithmeticRight8(0xFA, 0) == 0xFA); //when shifting 0, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.ShiftArithmeticRight8(0xFA, 32) == 0xFA); //when shifting 32, all flags should be unmodified (only bottom 5 bits used)
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 1);
    REQUIRE(cpu.freg.bits.pf == 1);
    REQUIRE(cpu.freg.bits.sf == 0);
    REQUIRE(cpu.freg.bits.of == 0);


    REQUIRE(cpu.ShiftArithmeticRight8(B8(11011100), 1) == B8(11101110));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.ShiftArithmeticRight16(B16(11011100, 00001100), 1) == B16(11101110, 00000110));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);

    REQUIRE(cpu.ShiftArithmeticRight32(B32(11011100, 00001100, 00000000, 00001001), 1) == 
        B32(11101110, 00000110, 00000000, 00000100));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0);
    REQUIRE(cpu.freg.bits.sf == 1);

    REQUIRE(cpu.ShiftArithmeticRight32(0xF00FABCC, 4) == 0xFF00FABC);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.zf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //undefined, but x86lib sets it to 0
}

TEST_CASE("RotateLeft", "[math ops]"){
    x86CPU cpu;
    REQUIRE(cpu.RotateLeft8(B8(11011101), 1) == B8(10111011));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); // MSB(dest) ^ CF; 1 ^ 1

    REQUIRE(cpu.RotateLeft8(B8(01011101), 1) == B8(10111010));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 1); // 1 ^ 0

    REQUIRE(cpu.RotateLeft8(0, 1) == 0); 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //0 ^ 0

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateLeft8(0xFA, 0) == 0xFA); //when shifting 0, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0);
    cpu.freg.bits.of = 1;
    REQUIRE(cpu.RotateLeft8(0xFA, 16) == 0xFA); //when shifting 16, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1);


    REQUIRE(cpu.RotateLeft8(B8(11011100), 1) == B8(10111001));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); // 1 ^ 1

    REQUIRE(cpu.RotateLeft16(B16(10011100, 00001100), 1) == B16(00111000, 00011001));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1); // 1 ^ 0

    REQUIRE(cpu.RotateLeft32(B32(01011100, 00001100, 00000000, 00001001), 1) == 
        B32(10111000, 00011000, 00000000, 00010010));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 1); // 0 ^ 1

    REQUIRE(cpu.RotateLeft32(0xF00FABCC, 4) == 0x00FABCCF);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); //undefined, but x86lib sets it to 0
}

TEST_CASE("RotateCarryLeft", "[math ops]"){
    x86CPU cpu;
    cpu.freg.bits.cf = 0;
    REQUIRE((int)cpu.RotateCarryLeft8(B8(11011101), 1) == B8(10111010));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); // MSB(dest) ^ CF; 1 ^ 1

    REQUIRE(cpu.RotateCarryLeft8(B8(01011101), 1) == B8(10111011));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 1); // 0 ^ 1

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateCarryLeft8(0, 1) == 1); 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //0 ^ 0

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateCarryLeft8(0xFA, 0) == 0xFA); //when shifting 0, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0);
    cpu.freg.bits.of = 1;
    REQUIRE(cpu.RotateCarryLeft8(0xFA, 16) == 0xFA); //when shifting 16, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1);
    cpu.freg.bits.cf = 0;
    REQUIRE(cpu.RotateCarryLeft8(B8(11011100), 1) == B8(10111000));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); // 1 ^ 1

    REQUIRE(cpu.RotateCarryLeft16(B16(10011100, 00001100), 1) == B16(00111000, 00011001));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1); // 1 ^ 0

    REQUIRE(cpu.RotateCarryLeft32(B32(01011100, 00001100, 00000000, 00001001), 1) == 
        B32(10111000, 00011000, 00000000, 00010011));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 1); // 0 ^ 1

    cpu.freg.bits.cf = 0;
    REQUIRE(cpu.RotateCarryLeft32(0xF00FABCC, 4) == 0x00FABCC7);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); //undefined, but x86lib sets it to 0

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateCarryLeft32(0xF00FABCC, 4) == 0x00FABCCF);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); //undefined, but x86lib sets it to 0
}

TEST_CASE("RotateRight", "[math ops]"){
    x86CPU cpu;
    REQUIRE(cpu.RotateRight8(B8(11011101), 1) == B8(11101110));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); // MSB1(dest) ^ MSB2(dest) 1 ^ 1

    REQUIRE(cpu.RotateRight8(B8(01011101), 1) == B8(10101110));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1); // 0 ^ 1

    REQUIRE(cpu.RotateRight8(0, 1) == 0); 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //0 ^ 0

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateRight8(0xFA, 0) == 0xFA); //when shifting 0, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0);
    cpu.freg.bits.of = 1;
    REQUIRE(cpu.RotateRight8(0xFA, 16) == 0xFA); //when shifting 16, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1);


    REQUIRE(cpu.RotateRight8(B8(11011100), 1) == B8(01101110));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 1); // 0 ^ 1

    REQUIRE(cpu.RotateRight16(B16(00011100, 00001100), 1) == B16(00001110, 00000110));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 0); // 0 ^ 0

    REQUIRE(cpu.RotateRight32(B32(01011100, 00001100, 00000000, 00001001), 1) == 
        B32(10101110, 00000110, 00000000, 00000100));

    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1); // 0 ^ 1

    REQUIRE(cpu.RotateRight32(0xF00FABCC, 4) == 0xCF00FABC);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); //undefined, but x86lib sets it to 0
}

TEST_CASE("RotateCarryRight", "[math ops]"){
    x86CPU cpu;
    cpu.freg.bits.cf = 0;
    REQUIRE((int)cpu.RotateCarryRight8(B8(11011101), 1) == B8(01101110));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1); // MSB(dest) ^ MSB2(dest); 0^1

    REQUIRE((int)cpu.RotateCarryRight8(B8(01011101), 1) == B8(10101110));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1); // 0 ^ 1

    REQUIRE((int)cpu.RotateCarryRight8(B8(01011110), 1) == B8(10101111));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 1); // 0 ^ 1

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateCarryRight8(0, 1) == 0x80); 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 1); //1 ^ 0

    REQUIRE(cpu.RotateCarryRight8(0, 1) == 0); 
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 0); //0 ^ 0

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateCarryRight8(0xFA, 0) == 0xFA); //when shifting 0, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0);
    cpu.freg.bits.of = 1;
    REQUIRE(cpu.RotateCarryRight8(0xFA, 16) == 0xFA); //when shifting 16, all flags should be unmodified 
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 1);
    cpu.freg.bits.cf = 0;
    REQUIRE(cpu.RotateCarryRight8(B8(01011100), 1) == B8(00101110));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 0); // 0 ^ 0

    REQUIRE(cpu.RotateCarryRight16(B16(10011100, 00001100), 1) == B16(01001110, 00000110));
    REQUIRE(cpu.freg.bits.cf == 0);
    REQUIRE(cpu.freg.bits.of == 1); // 1 ^ 0

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateCarryRight32(B32(11011100, 00001100, 00000000, 00001001), 1) == 
        B32(11101110, 00000110, 00000000, 00000100));
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); // 1 ^ 1

    cpu.freg.bits.cf = 0;
    REQUIRE(cpu.RotateCarryRight32(0xF00FABCC, 4) == 0x8F00FABC);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); //undefined, but x86lib sets it to 0

    cpu.freg.bits.cf = 1;
    REQUIRE(cpu.RotateCarryRight32(0xF00FABCC, 4) == 0x9F00FABC);
    REQUIRE(cpu.freg.bits.cf == 1);
    REQUIRE(cpu.freg.bits.of == 0); //undefined, but x86lib sets it to 0
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




