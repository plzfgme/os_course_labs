/*
 * 本文件实现vga的相关功能，清屏和屏幕输出
 * clear_screen和append2screen必须按照如下借口实现
 * 可以增加其他函数供clear_screen和append2screen调用
 */
extern void outb (unsigned short int port_to, unsigned char value);
extern unsigned char inb(unsigned short int port_from);

struct vga_char{
    char ch: 8;
    char fc: 4;
    char bc: 4;
};

static struct vga_char * const VGA_BASE = (struct vga_char*)0xb8000;

struct coord
{
    int x;
    int y;
};

static struct coord cursor = {0, 0};

void set_cursor(void) {
    unsigned short tmp;

    tmp = cursor.y * 80 + cursor.x;

    outb(0x3d4, 0xe);
    outb(0x3d5, tmp >> 8);

    outb(0x3d4, 0xf);
    outb(0x3d5, tmp);
}

void clear_screen(void) {
    int x, y;
    
    struct vga_char blank = {' ', 0xf, 0};

    for (y = 0; y < 25; y++){
        for (x = 0; x < 80; x++){
            VGA_BASE[y*80 + x] = blank;
        }
    }

    cursor.x = 0;
    cursor.y = 0;
    set_cursor();
}

void *memcpy(void *dest, const void *src, unsigned int count){
    const unsigned char *sp = (const unsigned char *)src;
    unsigned char *dp = (unsigned char *)dest;

    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, unsigned int count){
    unsigned short *temp = (unsigned short *)dest;

    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

void scroll(void) {
    unsigned short blank = ' ' | ((0<<4)|7)<<8;
  
    memcpy (VGA_BASE, VGA_BASE + 80, 80*24*2);
    memsetw((unsigned short *)VGA_BASE + 80*24, blank, 80);
    cursor.y--;
    set_cursor();
}

void putc_color(char ch, int color) {
    switch (ch){
        case '\r': 
            cursor.x = 0; 
            break;
        case '\n': 
            cursor.y = cursor.y + 1; 
            cursor.x = 0;   
            break;
        case '\b': 
            cursor.y -= (cursor.x == 0)?1:0;
            cursor.x = (cursor.x + 80 - 1)%80;
            VGA_BASE[cursor.y*80 + cursor.x].ch = ' ';
            VGA_BASE[cursor.y*80 + cursor.x].fc = 0xf;
            VGA_BASE[cursor.y*80 + cursor.x].bc = color;
            break;
        case '\t': 
            do {
                VGA_BASE[cursor.y*80 + cursor.x].ch = ' ';
                VGA_BASE[cursor.y*80 + cursor.x].fc = 0xf;
                VGA_BASE[cursor.y*80 + cursor.x].bc = color;
                cursor.y += (cursor.x + 1) / 80;
                cursor.x = (cursor.x + 1) % 80;
            } while (cursor.x % 4 != 0); 
            break;
        default: 
            VGA_BASE[cursor.y*80 + cursor.x].ch = ch;
            VGA_BASE[cursor.y*80 + cursor.x].fc = 0xf;
            VGA_BASE[cursor.y*80 + cursor.x].bc = color;
            cursor.y += (cursor.x + 1) / 80;
            cursor.x = (cursor.x + 1) % 80;      
    }

    if (cursor.y >= 25) {
        scroll();
    }
    set_cursor();
}

void append2screen(char *str,int color) { 
    for (; *str != '\0'; str++) {
        putc_color(*str, color);
    }
}