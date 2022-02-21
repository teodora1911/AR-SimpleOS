bits 32

global gdtDescriptor
global CODE_SEGMENT
global DATA_SEGMENT

section .text
align 4096

section .rodata
align 4096

gdtStart:

gdtNull:
    dd 0x0
    dd 0x0

gdtCode:
    dw 0xffff    ; limit 1
    dw 0x0       ; base 1
    db 0x0       ; base 2
    db 10011010b ; 0x9a - access byte
    db 10101111b ; 0xaf - flags + limit 2
    db 0x0       ; base 3

gdtData:
    dw 0xffff    ; limit 1
    dw 0x0       ; base 1
    db 0x0       ; base 2
    db 10010010b ; 0x92 - access byte
    db 10101111b ; 0xaf - flags + limit 2
    db 0x0       ; base 3

gdtEnd:

gdtDescriptor:
    dw gdtEnd - gdtStart - 1
    dq gdtStart

CODE_SEGMENT equ gdtCode - gdtStart
DATA_SEGMENT equ gdtData - gdtStart
