./riscv/toolchain/bin/riscv64-unknown-elf-as -march=rv32i -mno-relax test.asm -o test.o
./riscv/toolchain/bin/riscv64-unknown-elf-ld -m elf32lriscv -no-relax -nostdlib  test.o -o test.out 
