; simple.asm
; this program just returns 42
; you can use the command readelf to see its content and determine
; the main function entry point (i.e. _start function) address
BITS 64
GLOBAL _start
SECTION .text
    jmp     $       ; jump to current address -> infinite loop
_start:
    mov     eax, 1  ; systemcall number 1 -> exit
    mov     ebx, 42 ; parameter of system call -> exit with code 42
    int     0x80    ; interrupt executing the system call with number stored in eax