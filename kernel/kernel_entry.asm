[bits 32]
global kernel_start
extern kernel_main

kernel_start:
    call kernel_main
hang:
    jmp hang
