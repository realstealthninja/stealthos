.macro pushaq
    push %rax
    push %rbx
    push %rcx
    push %rdx 
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
.endm

.macro popaq
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
.endm


.macro isr_stub num:req, err
.align 16
isr_stub_\num\():
    .ifb \err
        pushq $0 # dummy error code
    .endif
    pushq $\num
    jmp interrupt_stub
.endm


.section .text
.extern interrupt_dispatcher
interrupt_stub:
    pushaq

    mov %rsp, %rdi
    call interrupt_dispatcher
    mov %rax, %rsp
    
    popaq
    add $16, %rsp   # using the convention suggested by osdev-notes on github
    iretq

# the macros with the second arugment have a predefined error thus
# dummy error is unnecessary
isr_stub 0
isr_stub 1
isr_stub 2
isr_stub 3
isr_stub 4
isr_stub 5
isr_stub 6
isr_stub 7
isr_stub 8  1
isr_stub 9
isr_stub 10 1
isr_stub 11 1
isr_stub 12 1
isr_stub 13 1
isr_stub 14 1
isr_stub 15
isr_stub 16
isr_stub 17 1
isr_stub 18
isr_stub 19
isr_stub 20
isr_stub 21
isr_stub 22
isr_stub 23
isr_stub 24
isr_stub 25
isr_stub 26
isr_stub 27
isr_stub 28
isr_stub 29
isr_stub 30 1
isr_stub 31
isr_stub 240


.section .data

.global isr_stub_table
isr_stub_table:
.rept 32
    .quad isr_stub_\+
.endr
