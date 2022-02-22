bits 32

global enablePaging

section .text
enablePaging:
    call setPageTables

    ; podesiti tako da registar cr3 pokazuje na pageMapLevel4 (specifikacija)
    mov eax, pageMapLevel4
    mov cr3, eax

    ; long mode is an extension of Physical Address Extension (PAE), so we need to enable PAE first
    ; enable PAE-flag in cr4 (Physical Address Extension)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit in the EFER MSR (Model Specific Register)
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

setPageTables:
    PRESENT equ 1 << 0
    WRITABLE equ 1 << 1
    USER_ACCESSIBLE equ 1 << 2
    HUGE_TABLE equ 1 << 7

    mov eax, pageDirectoryPointerTable
    or eax, PRESENT | WRITABLE | USER_ACCESSIBLE
    mov [pageMapLevel4], eax

    mov eax, pageDirectory
    or eax, PRESENT | WRITABLE | USER_ACCESSIBLE
    mov [pageDirectoryPointerTable + 8], eax

    mov eax, pageTable
    or eax, PRESENT | WRITABLE | USER_ACCESSIBLE
    mov [pageDirectory + 8], eax

    mov eax, 0
    or eax, PRESENT | WRITABLE | USER_ACCESSIBLE | HUGE_TABLE
    mov [pageDirectoryPointerTable], eax

    mov eax, 0
    or eax, PRESENT | WRITABLE | USER_ACCESSIBLE | HUGE_TABLE
    mov [pageDirectory], eax

    mov eax, 0
    or eax, PRESENT | WRITABLE | USER_ACCESSIBLE
    mov [pageTable], eax

    ret

section .bss
align 4096
; svaka tabela sadrzi 512 unosa od po 8 bajtova
; zbog toga svaka staje u jednu stranicu (512 * 8 = 4096)

    pageMapLevel4 resb 512*8
    pageDirectoryPointerTable resb 512*8
    pageDirectory resb 512*8
    pageTable resb 512*8