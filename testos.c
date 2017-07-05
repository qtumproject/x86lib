
typedef unsigned int uint32_t;
//#include <stdint.h>

void start(){
    uint32_t* tmp=0xFFFF0000;
    *tmp=0xDEADBEEF;
    while(1);
}
