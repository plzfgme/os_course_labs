#include "asm.h"
#include "type.h"

inline void outb (uint16_t port, uint8_t data){
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}
