bits 32

global runChecks

BOOTLOADER_MAGIC equ 0x36D76289 ; according to specification

section .text
runChecks:
    call checkMultiboot
    call checkCPUID
    call checkLongMode
    ret

checkMultiboot:
    cmp eax, BOOTLOADER_MAGIC
    jne .noMultiboot
    ret
.noMultiboot:
    mov al, "M"
    jmp error

checkCPUID:
    ; trying to reverse ID bit in EFLAGS register (21. bit)
    ; https://studfile.net/preview/1583052/page:17/
    ; if we are able to do so then cpuid is available

    ; stavljamo EFLAGS u EAX preko stack-a
    pushfd 
    pop eax

    ; pravimo njihovu kopiju u ECX da bismo kasnije mogli uporediti
    mov ecx, eax

    ; XOR da bismo okrenuli 21. bit
    xor eax, 1 << 21

    ; kopiramo novu vrijednost EAX registra u EFLAGS preko stacka
    push eax
    popfd

    ; opet stavljamo EFLAGS u EAX preko stack-a (ali sada je 21. bit izmijenjen ako je cpuid podrzan)
    pushfd
    pop eax

    ; vracamo staru verziju u EFLAGS koju smo sacuvali u ECX
    push ecx
    popfd

    ; poredimo EAX (koji treba da sadrzi verziju EFLAGS sa okrenutim 21. bitom)
    ; i ECX (koji sadrzi staru verziju EFLAGS)
    cmp eax, ecx
    ; ako nisu isti onda cpuid nije podrzan
    je .noCPUID
    ret
.noCPUID:
    mov al, "C"
    jmp error

checkLongMode:
    ; sada kada je cpuid dostupan, provjeravamo da li mozemo da koristimo long mode
    ; mozemo to detektovati samo ako koristimo "extended functions of cpuid" (> 0x80000000)
    mov eax, 0x80000000   ; implicit argument for cpuid
    cpuid                 ; get highest supported argument
    cmp eax, 0x80000001   ; it needs to be at least 0x80000001
    jb .noLongMode        ; it is less --> there is no long mode

    ; sad kada je "extended function" dostupna mozemo da je koristimo da bismo detektovali long mode
    mov eax, 0x80000001   ; argument for extended processor info
    cpuid                 ; returns various bits in ecx and edx
    test edx, 1 << 29     ; test if the LM-bit, which is bit 29, is set in the D-register
    jz .noLongMode        ; they are not --> there is no long mode
    ret
.noLongMode:
    mov al, "L"
    jmp error

error: 
    ; 0xb8000 - VGA text buffer
    ; word format : background color(4 b), foreground color (4 b), character code (in ascii)
    ; https://web.archive.org/web/20160326064341/http://jamesmolloy.co.uk/tutorial_html/3.-The%20Screen.html
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte [0xb800a], al
    cli 

.hang:
    hlt
    jmp .hang
