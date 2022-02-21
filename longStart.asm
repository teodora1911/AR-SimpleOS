bits 64

global longModeEntry

extern DATA_SEGMENT
extern main

section .text
align 4096

longModeEntry:
    mov qword rax, DATA_SEGMENT
    mov word ss, rax
    mov word ds, rax
    mov word es, rax
    mov word fs, rax
    mov word gs, rax

    ; enable SSE
    mov rax, cr0
    and ax, 0xfffb
    or ax, 0x2
    mov cr0, rax

    mov rax, cr4
    or ax, 3 << 9
    mov cr4, rax

    call main

    .noSSE:
        mov rax, 0x2f522f522f45
        mov [0xb8000], rax

    cli
    hlt