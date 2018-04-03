DEF='\033[0m'
GRN='\033[0;32m'
RED='\033[0;31m'

../ia32disassembler decoder/Smoke.bin > decoder/Smoke.asm_test
cmp --silent decoder/Smoke.asmx decoder/Smoke.asm_test
if [ $? -eq 0 ];
then
   printf "${GRN}Decoder smoke test passed${DEF}\n"
else
   printf "${RED}Decoder smoke test failed${DEF}\n"
fi

