#include "x86test.h"


TEST_CASE("etc op_bound_rW_mW", "[etc]") {
    x86Tester test;
    x86Checkpoint check = test.LoadCheckpoint();
    check.SetReg32(EBX, HIGH_SCRATCH_ADDRESS);
	check.SetHighScratch32(0,1);
	check.SetHighScratch32(4,100);
    test.Apply(check);

	bool fExcept=false;
	try{
	    test.Run(
		"mov eax, 8\n"
		"bound eax, [ebx]\n"
		"jmp _end\n");
	}catch(...){
		fExcept=true;
	}
	
    REQUIRE(fExcept == false);

}



