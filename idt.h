#ifndef IDT_H
#define IDT_H

typedef struct {
    uint16_t offsetLow;
    uint16_t segmentSelector; // code segment kernela
    uint8_t ist;              // treba da bude 0 ukoliko ne koristimo Interrupt Stack Table (koji se nalazi u Task State Segmentu) - ne koristimo
    uint8_t attributes;       // type and attributes; flags
    uint16_t offsetMid;
    uint32_t offsetHigh;
    uint32_t reserved;        // treba da budu 0
} __attribute__((packed)) IDTGate;

typedef struct {
    uint16_t limit;           // velicina IDT
    uint64_t base;            // pocetna adresa
} __attribute__((packed)) IDTDescriptor;

typedef struct {
    uint64_t ds;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rdx, rcx, rbx, rax; 
    uint64_t i_number, error_code;              // error_code u nekim postavljen, u nekima ne --> mi postavljamo ako nema i redni broj rutine
    uint64_t rip, cs, rflags, rsp, ss;          // postavljeno na stack kada se pozove ISR - prema specifikaciji
} interruptFrame;

extern void IDTSetGate(uint64_t number, uint64_t offset, uint16_t selector, uint8_t attributes);
extern void IRQRemap();
extern void IDTInstall();
extern void ISRInstall();
extern void IRQInstall();

extern void isrHandler();
extern void irqHandler();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif