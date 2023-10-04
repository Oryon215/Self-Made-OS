[bits 32]
[extern main]

global _start     ; Add this line to define _start as the entry point

section .text
_start:
    call main
    jmp $