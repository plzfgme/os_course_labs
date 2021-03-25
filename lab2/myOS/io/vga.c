#include "vga.h"
#include "asm.h"
#include "type.h"

void set_cursor(int x, int y) {
    uint16_t tmp;

    tmp = y * 80 + x;

    outb(0x3d4, 0xe);
    outb(0x3d5, tmp >> 8);

    outb(0x3d4, 0xf);
    outb(0x3d5, tmp);
}

void clear_screen(void){  
    int x, y;
    struct vga_char* vga_mem = (struct vga_char*)0xb8000;
    
    struct vga_char blank = {' ', 7, 0};

    for (y = 0; y < 25; y++){
        for (x = 0; x < 80; x++){
            vga_mem[y*80 + x] = blank;
        }
    }

    set_cursor(0, 0);
}


