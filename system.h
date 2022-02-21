#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdint.h>

// main
extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);
extern void io_wait(void);

extern int strlen(const char* str);

extern void VGAInitialize(void);
extern void puts(unsigned char* text);
extern void putch(unsigned char c);
extern void changeDisplayColor(int counter);

// timer
extern void timerHandler();

// keyboard
extern void keyboardHandler();
extern void setEnv();
extern void sum();
extern void sub();

#endif