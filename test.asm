.globl _start

.data
    lol:
        .word 12
    lols:
        .asciz "to fibonachi or not fibonachi, that is the question"
.text
_start:
    addi x1, x0, 0 #pred prev number
    addi x2, x0, 1 #prev number
    addi x9, x0, 1 # counter
    addi x10, x0, 2 # write which number u want (counting from one (the first))
    # answer will be in x9

    beq x9, x10, first_num
ok:
    add x3, x1, x2
    addi x1, x2, 0
    addi x2, x3, 0
    addi x9, x9, 1 # iterate counter
    bne x9, x10, ok
    addi x9, x3, 0
    addi x10, x0, 0xDD # treat as a flag of ending
    ebreak
first_num:
    addi x9, x0, 0
    addi x10, x0, 0xDD
    ebreak
