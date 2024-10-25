    global outb
    section .text:
    align 4
    outb:
        mov al, [esp + 8]
        mov dx, [esp + 4]
        out dx, al
        ret