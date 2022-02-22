#include <stdint.h>
#include "system.h"
#include "idt.h"

IDTGate idt[256];
IDTDescriptor IDTPointer;

extern void loadIDT();

void IDTSetGate(uint64_t number, uint64_t offset, uint16_t selector, uint8_t attributes){
    idt[number].offsetLow       = (uint16_t)(offset & 0x000000000000ffff);
    idt[number].offsetMid       = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    idt[number].offsetHigh      = (uint32_t)((offset & 0xffffffff00000000) >> 32);

    idt[number].segmentSelector = selector;     // uvijek je 0x08 - code segment GDT
    idt[number].ist             = 0;            
    idt[number].attributes      = attributes;   // 0x8e - E --> interrupt gate; F --> trap gate
    idt[number].reserved        = 0;        
}

void IDTInstall(){
    IDTPointer.limit = (sizeof(IDTGate) * 256) - 1;
    IDTPointer.base = (uint64_t)idt;

    //puts("In IDT Install func! ");

    uint64_t i;
    for(i = 0; i < 256; ++i){
        IDTSetGate(i, 0, 0, 0);
    }

    //puts("Null all gates! ");

    loadIDT();
}

/*
unsigned char* exceptionMessages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "BreakPoint",
    "Into Detected Overflow",
    "Out Of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocesor Exception",
    "Double Fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Procetion Fault",
    "Page Fault",
    "Unknown Interrupt Exception",
    "Coprocessor Fault",
    "Alignment Check Exception",
    "Machine Check Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};
*/

void ISRInstall(){
    IDTSetGate(0, (uint64_t)isr0, 0x08, 0x8E);
    IDTSetGate(1, (uint64_t)isr1, 0x08, 0x8E);
    IDTSetGate(2, (uint64_t)isr2, 0x08, 0x8E);
    IDTSetGate(3, (uint64_t)isr3, 0x08, 0x8E);
    IDTSetGate(4, (uint64_t)isr4, 0x08, 0x8E);
    IDTSetGate(5, (uint64_t)isr5, 0x08, 0x8E);
    IDTSetGate(6, (uint64_t)isr6, 0x08, 0x8E);
    IDTSetGate(7, (uint64_t)isr7, 0x08, 0x8E);
    IDTSetGate(8, (uint64_t)isr8, 0x08, 0x8E);
    IDTSetGate(9, (uint64_t)isr9, 0x08, 0x8E);
    IDTSetGate(10, (uint64_t)isr10, 0x08, 0x8E);
    IDTSetGate(11, (uint64_t)isr11, 0x08, 0x8E);
    IDTSetGate(12, (uint64_t)isr12, 0x08, 0x8E);
    IDTSetGate(13, (uint64_t)isr13, 0x08, 0x8E);
    IDTSetGate(14, (uint64_t)isr14, 0x08, 0x8E);
    IDTSetGate(15, (uint64_t)isr15, 0x08, 0x8E);
    IDTSetGate(16, (uint64_t)isr16, 0x08, 0x8E);
    IDTSetGate(17, (uint64_t)isr17, 0x08, 0x8E);
    IDTSetGate(18, (uint64_t)isr18, 0x08, 0x8E);
    IDTSetGate(19, (uint64_t)isr19, 0x08, 0x8E);
    IDTSetGate(20, (uint64_t)isr20, 0x08, 0x8E);
    IDTSetGate(21, (uint64_t)isr21, 0x08, 0x8E);
    IDTSetGate(22, (uint64_t)isr22, 0x08, 0x8E);
    IDTSetGate(23, (uint64_t)isr23, 0x08, 0x8E);
    IDTSetGate(24, (uint64_t)isr24, 0x08, 0x8E);
    IDTSetGate(25, (uint64_t)isr25, 0x08, 0x8E);
    IDTSetGate(26, (uint64_t)isr26, 0x08, 0x8E);
    IDTSetGate(27, (uint64_t)isr27, 0x08, 0x8E);
    IDTSetGate(28, (uint64_t)isr28, 0x08, 0x8E);
    IDTSetGate(29, (uint64_t)isr29, 0x08, 0x8E);
    IDTSetGate(30, (uint64_t)isr30, 0x08, 0x8E);
    IDTSetGate(31, (uint64_t)isr31, 0x08, 0x8E);
}

void isrHandler(){
    puts("Exception! \n");
    puts("Halt!\n");
    for(;;);
}

void irqRemap(){
    // inicijalizacija command
    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    // postavljamo offset
    outb(0x21, 0x20);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();
    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(0x21, 0x04);
    io_wait();
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
    outb(0xA1, 0x02);
    io_wait();
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // mask everything except keyboard
    //outb(0x21, 0xFD);
    //outb(0xA1, 0xFF);

    //puts("Remap! ");
}

void IRQInstall(){
    irqRemap();

    IDTSetGate(32, (uint64_t)irq0, 0x08, 0x8E);
    IDTSetGate(33, (uint64_t)irq1, 0x08, 0x8E);
    IDTSetGate(34, (uint64_t)irq2, 0x08, 0x8E);
    IDTSetGate(35, (uint64_t)irq3, 0x08, 0x8E);
    IDTSetGate(36, (uint64_t)irq4, 0x08, 0x8E);
    IDTSetGate(37, (uint64_t)irq5, 0x08, 0x8E);
    IDTSetGate(38, (uint64_t)irq6, 0x08, 0x8E);
    IDTSetGate(39, (uint64_t)irq7, 0x08, 0x8E);
    IDTSetGate(40, (uint64_t)irq8, 0x08, 0x8E);
    IDTSetGate(41, (uint64_t)irq9, 0x08, 0x8E);
    IDTSetGate(42, (uint64_t)irq10, 0x08, 0x8E);
    IDTSetGate(43, (uint64_t)irq11, 0x08, 0x8E);
    IDTSetGate(44, (uint64_t)irq12, 0x08, 0x8E);
    IDTSetGate(45, (uint64_t)irq13, 0x08, 0x8E);
    IDTSetGate(46, (uint64_t)irq14, 0x08, 0x8E);
    IDTSetGate(47, (uint64_t)irq15, 0x08, 0x8E);
}

void irqHandler(){
    puts("Unhandled ISR!\n");
    outb(0x20, 0x20);
    outb(0xA0, 0x20);   
}