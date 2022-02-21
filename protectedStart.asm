bits 32

global start

extern runChecks
extern enablePaging
extern CODE_SEGMENT
extern gdtDescriptor
extern longModeEntry

section .text 
align 4096

start:
    mov esp, stackTop

    call runChecks
    call enablePaging
    lgdt [gdtDescriptor]
    jmp CODE_SEGMENT:longModeEntry

    cli
.hang:
    hlt
    jmp .hang

section .data
align 4096
    var dd 0

section .bss
align 4096 ; each table is 4KB
stackBottom:
    resb 65536*1024 ; 64 MB
stackTop: