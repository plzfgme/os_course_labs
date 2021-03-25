.globl _start
.text
.code32

_start:
    jmp establish_stack
    
dead:   
    jmp dead                        # Never here
    
# Set up the stack

establish_stack:
    # movl    $0x2f652f48, %eax
    movl    $0xbfffff, %eax
    movl    %eax, %esp              # set stack pointer
    movl    %eax, %ebp              # set base pointer

    jmp     zero_bss
    
# Zero out the BSS segment
zero_bss:
    cld                                           # make direction flag count up
    movl    $_end, %ecx                 # find end of .bss
    movl    $_bss_start, %edi       # edi = beginning of .bss
    subl    %edi, %ecx                  # ecx = size of .bss in bytes
    shrl    %ecx                            # size of .bss in longs
    shrl    %ecx
    xorl    %eax, %eax                  # value to clear out memory
    repne                                   # while ecx != 0
    stosl                                       # clear a long in the bss

    jmp     to_osStart

# Transfer control to osStart

to_osStart:
    # mov   $0xb8000, %eax
    # movw   $0x2f65, (%eax)
    call    osStart
    hlt
    
shut_down:
    jmp     shut_down       # Never here
