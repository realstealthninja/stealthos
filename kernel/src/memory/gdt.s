.section .text
.global reload_seg
reload_seg:
    push $0x08
    leaq reload_cs, %rax
    push %rax
    retfq

reload_cs:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %gs
    ret

