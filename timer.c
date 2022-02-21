#include "system.h"
#include "idt.h"

int numberOfTicks = 0;
int color = 1;

void timerHandler(){
    ++numberOfTicks;
    //changeDisplayColor(color);
    //++color;
    if(numberOfTicks % 18 == 0){
        //puts("One second passed! ");
        changeDisplayColor(color);
        ++color;
    }

    outb(0x20, 0x20);
}