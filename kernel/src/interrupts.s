.section .text

.extern exception_handler # common exception_handler 

.macro isr_err_stub num
isr_stub_\num\():
    call exception_handler
    iretq
.endm

.macro isr_no_err_stub num
isr_stub_\num\():
    call exception_handler
    iretq
.endm

.macro pushaq
    push %rax
    push %rbx
    push %rcx
    push %rdx 
    push %rsi
    push %rdi
    push %rsp
    push %rbp
.endm

.macro popaq
    pop %rax
    pop %rbx
    pop %rcx
    pop %rdx 
    pop %rsi
    pop %rdi
    pop %rsp
    pop %rbp
.endm


# generic irq handler
#
# irq_handlers is an array of function (interrupt request handlers)
# which can be used to index into to get our handler
#
.extern irq_handlers 
.macro irq_stub num
irq_stub_\num\():
    pushaq                            # # push registers on the the stack

    leaq        irq_handlers, %r15    # # load irq_handlers into r15
    movq        \num(, %r15, 8), %r15 # # set r15 to be the address of our irq handler 

    cld                               # # calling convention
    call        *%r15                 # # call the handler

    popaq                             # # get our registers back from the stack
    iretq                             # # interrupt over
.endm 


# exception service routines
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

# IRQs
irq_stub        32
irq_stub        33
irq_stub        34
irq_stub        35
irq_stub        36
irq_stub        37
irq_stub        38


.section .data

.global isr_stub_table


isr_stub_table:               # macro for loop to set all the random error stubs
.rept           31            # add all errors/traps to the isr_stub_table 
.quad isr_stub_\+
.endr
.quad irq_stub_32
.quad irq_stub_33
.quad irq_stub_34
.quad irq_stub_35
.quad irq_stub_36
.quad irq_stub_37
.quad irq_stub_38

