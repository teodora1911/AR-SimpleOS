bits 32

; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html

section .data

MAGIC_NUMBER equ 0xE85250D6
HEADER_LENGTH equ headerEnd - headerStart

section .multiboot_header
headerStart:
; header magic fields
    ; magic number
    dd MAGIC_NUMBER
    ; architecture - 32-bit (protected) mode of i386
    dd 0
    ; header length
    dd HEADER_LENGTH
    ; checksum - 32-bit unsigned value which, when added to the other magic fields (magic, architecture and header length), must have a 32-bit unsigned sum of zero.
    dd 0x100000000 - (MAGIC_NUMBER + HEADER_LENGTH)

; header tags - da bismo oznacili da nemamo vise podataka
    ; type
    dw 0
    ; flags
    dw 0
    ; size
    dd 8
headerEnd: