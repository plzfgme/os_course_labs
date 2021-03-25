.globl start
/*multiboot header magic number*/
MULTIBOOT_HEADER_MAGIC = 0x1BADB002
/*multiboot header flags*/
MULTIBOOT_HEADER_FLAGS = 0x00001007

.section ".multiboot_header"
/*magic number*/
.long   MULTIBOOT_HEADER_MAGIC
/*flags*/
.long   MULTIBOOT_HEADER_FLAGS
/*checksum*/
.long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
/*no need in elf format*/
.long   0
.long   0
.long   0
.long   0
.long   0
/*graphic fields*/  
.long   0
.long   1024
.long   768
.long   32

.text

.code32

start:
    movl   $0xbfffff, %eax
    movl   $0xb8000,0(%eax)
    mov    0(%eax), %eax
    movw   $0x2f65, 0(%eax)

    movw $0x3F8, %dx  //串口位置
    outl %eax, %dx

    /*
    movl $0x2f652f48, 0xb8000
    movl $0x2f6c2f6c, 0xb8004
    movl $0x2f772f6f, 0xb8008
    movl $0x2f722f6f, 0xb800c
    movl $0x2f642f6c, 0xb8010
    movl $0x2f502f2c, 0xb8014
    movl $0x2f312f42, 0xb8018
    movl $0x2f302f38, 0xb801c
    movl $0x2f312f36, 0xb8020
    movl $0x2f312f34, 0xb8024
    movl $0x2f5f2f34, 0xb8028
    movl $0x2f752f67, 0xb802c
    movl $0x2f792f6f, 0xb8030
    movl $0x2f742f75, 0xb8034
    movl $0x2f6f2f61, 0xb8038


    outb %al, %dx
    movb $0x65, %al
    outb %al, %dx
    movb $0x6c, %al
    outb %al, %dx
    movb $0x6c, %al
    outb %al, %dx
    movb $0x6f, %al
    outb %al, %dx
    movb $0x77, %al
    outb %al, %dx
    movb $0x6f, %al
    outb %al, %dx
    movb $0x72, %al
    outb %al, %dx
    movb $0x6c, %al
    outb %al, %dx
    movb $0x64, %al
    outb %al, %dx
    movb $0x2c, %al
    outb %al, %dx
    movb $0x50, %al
    outb %al, %dx
    movb $0x42, %al
    outb %al, %dx
    movb $0x31, %al
    outb %al, %dx
    movb $0x38, %al
    outb %al, %dx
    movb $0x30, %al
    outb %al, %dx
    movb $0x36, %al
    outb %al, %dx
    movb $0x31, %al
    outb %al, %dx
    movb $0x34, %al
    outb %al, %dx
    movb $0x31, %al
    outb %al, %dx
    movb $0x34, %al
    outb %al, %dx
    movb $0x5f, %al
    outb %al, %dx
    movb $0x67, %al
    outb %al, %dx
    movb $0x75, %al
    outb %al, %dx
    movb $0x6f, %al
    outb %al, %dx
    movb $0x79, %al
    outb %al, %dx
    movb $0x75, %al
    outb %al, %dx
    movb $0x74, %al
    outb %al, %dx
    movb $0x61, %al
    outb %al, %dx
    movb $0x6f, %al
    outb %al, %dx
    */

    /*暂停*/
h:
    hlt
    jmp h


