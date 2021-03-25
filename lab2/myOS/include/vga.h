#ifndef __VGA_H_
#define __VGA_H_



struct vga_char{
    char ch: 8;
    char fc: 4; // foreground color
    char bc: 4; // background color
};

void set_cursor(int x, int y);

void clear_screen(void);

void scroll(void);

#endif