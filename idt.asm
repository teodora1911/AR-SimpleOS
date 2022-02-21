bits 64
section .text
align 4096

global loadIDT
extern IDTPointer

loadIDT:
    lidt [IDTPointer]
    ret

%macro PUSHALL 0
    push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro POPALL 0
    pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro


extern isrHandler
extern irqHandler

extern timerHandler
extern keyboardHandler

isrCommonStub: 
    PUSHALL
    mov ax, ds
    push rax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isrHandler

    pop rbx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    POPALL
    add rsp, 16
    iretq

;extern DATA_SEGMENT

irqCommonStub:
    PUSHALL
    mov ax, ds
    push rax

    mov ax, 0x10   ; DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irqHandler

    pop rax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    POPALL
    add rsp, 16
    iretq

%macro ISR_NO_ERRORCODE 1
    global isr%1

    isr%1:
        cli
        push byte 0         ; error code koji "nedostaje"
        push byte %1        ; interrupt id
        jmp isrCommonStub
%endmacro

%macro ISR_ERROR_CODE 1
    global isr%1

    isr%1:
        cli
        push byte %1 
        jmp isrCommonStub
%endmacro

%macro IRQ 2
    global irq%1

    irq%1:
        cli
        push byte 0         ; error code 
        push byte %2        ; interrupt id
        jmp irqCommonStub
%endmacro

ISR_NO_ERRORCODE 0
ISR_NO_ERRORCODE 1
ISR_NO_ERRORCODE 2
ISR_NO_ERRORCODE 3
ISR_NO_ERRORCODE 4
ISR_NO_ERRORCODE 5
ISR_NO_ERRORCODE 6
ISR_NO_ERRORCODE 7
ISR_ERROR_CODE   8 
ISR_NO_ERRORCODE 9
ISR_ERROR_CODE   10
ISR_ERROR_CODE   11
ISR_ERROR_CODE   12
ISR_ERROR_CODE   13
ISR_ERROR_CODE   14
ISR_NO_ERRORCODE 15
ISR_NO_ERRORCODE 16
ISR_NO_ERRORCODE 17
ISR_NO_ERRORCODE 18
ISR_NO_ERRORCODE 19
ISR_NO_ERRORCODE 20
ISR_NO_ERRORCODE 21
ISR_NO_ERRORCODE 22
ISR_NO_ERRORCODE 23
ISR_NO_ERRORCODE 24
ISR_NO_ERRORCODE 25
ISR_NO_ERRORCODE 26
ISR_NO_ERRORCODE 27
ISR_NO_ERRORCODE 28
ISR_NO_ERRORCODE 29
ISR_NO_ERRORCODE 30
ISR_NO_ERRORCODE 31

global irq0
irq0:
    cli
    push byte 0
    push byte 32

    PUSHALL
    mov ax, ds
    push rax

    mov ax, 0x10   ; DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call timerHandler

    pop rax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    POPALL
    add rsp, 16
    iretq

global irq1
irq1:
    cli
    push byte 0
    push byte 33

    PUSHALL
    mov ax, ds
    push rax

    mov ax, 0x10   ; DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call keyboardHandler

    pop rax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    POPALL
    add rsp, 16
    iretq

IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47