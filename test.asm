.globl _start

.data

.text
_start:
    addi x1, x0, 1
    addi x2, x0, 1
    addi x4, x0, 5
ok:
    add x3, x1, x2
    addi x1, x2, 0
    addi x2, x3, 0
    bne x3, x4, ok
    addi x5, x0, 1488
    ebreak