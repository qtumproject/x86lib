
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
//#include <stdint.h>

static inline void outb(uint16_t port, uint32_t val)
{
    asm volatile ( "outd %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint32_t inb(uint16_t port)
{
    uint32_t ret;
    asm volatile ( "ind %1, %0"
    : "=a"(ret)
    : "Nd"(port) );
    return ret;
}




void start(){
    uint32_t* tmp=0xFFFF0000;
    float x=0.001;
    *tmp=0xDEADBEEF;
    uint64_t test = 0xFFFF0000CCCCAAAA;
    uint64_t* test2 = 0xFFFF01234;
    test *= *test2;
    test+=20;
    test*=0xFFFF0000;
    uint64_t* tmp2=0xF0000;
    *tmp2 = test;
    float* y=0xFF000;
    *y = x * 0.2 / test;
    while(1);
}
