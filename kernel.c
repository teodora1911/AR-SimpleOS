/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#include <stdint.h>
#include "system.h"
#include "idt.h"

int attrib = 0x0f;

/*
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15
*/

unsigned short* textMemoryPointer;
int cursorX = 0;
int cursorY = 0;

int strlen(const char* str){
	int len;
    for(len = 0; *str != '\0'; ++str){
		++len;
	}
    return len;
}

uint8_t inb(uint16_t port){
	uint8_t rv;
    asm volatile ("inb %1, %0" : "=a" (rv) : "Nd" (port));
    return rv;
}

void outb(uint16_t port, uint8_t data){
    //asm volatile ("outb %1, %0" : : "Nd"(port), "a"(data));
	asm volatile ( "outb %0, %1" : : "a"(data), "Nd"(port) );
}

void io_wait(void){
    outb(0x80, 0);
}

static void setTextColor(unsigned char foregroundColor, unsigned char backgroundColor){
	attrib = (backgroundColor << 4) | (foregroundColor & 0x0f);
}

void VGAInitialize(void){
	textMemoryPointer = (unsigned short*)0xB8000;
	cursorX = 0;
	cursorY = 0;
	setTextColor(1, 0);
	for(int y = 0; y < 25; ++y){
		for(int x = 0; x < 80; ++x){
			const int index = y * 80 + x;
			unsigned char c = ' ';
			textMemoryPointer[index] = (c | (attrib << 8));
		}
	}
}

void moveCursor(){
	unsigned temp;
	temp = cursorY * 80 + cursorX;

	outb(0x3D4, 14);
	outb(0x3D5, temp >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, temp);
}

void putch(unsigned char c){
	unsigned short* where;
	unsigned att = attrib << 8;

	if(c == '\n'){
		cursorX = 0;
		cursorY++;
	} else if(c >= ' '){
		where = textMemoryPointer + (cursorY * 80 + cursorX);
		*where = c | att;
		cursorX++;
	}

	if(cursorX >= 80){
		cursorX = 0;
		cursorY++;
	}

	moveCursor();
}

void puts(unsigned char* text){
	int i;
	int length = strlen(text);
	for(i = 0; i < length; ++i){
		putch(text[i]);
	}
}

void changeDisplayColor(int counter){
	counter %= 16;
	if(counter == 0){
		counter = 1;
	}

	setTextColor(counter, 0);
	for(int y = 0; y < 25; ++y){
		for(int x = 0; x < 80; ++x){
			const int index = y * 80 + x;
			unsigned char ch = (unsigned char)(textMemoryPointer[index] & 0xFF);
			textMemoryPointer[index] = ch | (attrib << 8);
		}
	}
}

void main(void)
{
	VGAInitialize();
	IDTInstall();
	ISRInstall();
	IRQInstall();
	setEnv();

	asm volatile ("sti");

	for(;;);
}