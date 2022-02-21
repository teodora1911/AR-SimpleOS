#include <stdbool.h>
#include <stdint.h>
#include "system.h"
#include "idt.h"

#define DIGIT 50

unsigned char keyboardMap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
    'k', 'l', ';', 0, 0, 0, 0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.',
    0, 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int enterClickNumber = 0;
bool performedSum = false;

int firstNumber[DIGIT];
int secondNumber[DIGIT];
int result[DIGIT + 1];

int firstNumberSize = 0;
int secondNUmberSize = 0;

int counter = 0;

void keyboardHandler(){
    uint8_t scancode = inb(0x60);
    
    // ako je MSB bajta kojeg citamo setovan, to znaci da je taster upravo pusten
    if(scancode & 0x80){
        // ...
    } else if(enterClickNumber >= 2){
        putch(keyboardMap[scancode]);
    } else {
        unsigned char symbol = keyboardMap[scancode];
        if((symbol >= '0') && (symbol <= '9') && (counter < DIGIT)){
            int number = symbol - '0';
            if(enterClickNumber){
                secondNumber[secondNUmberSize++] = number;
            } else {
                firstNumber[firstNumberSize++] = number;
            }

            counter++;
            putch(symbol);
        } else if(symbol == '\n'){
            counter = 0;
            ++enterClickNumber;
            putch(symbol);

            if(enterClickNumber == 2){
                puts("Suma je : ");
                sum();
                putch('\n');
            }
        }
    }
    outb(0x20, 0x20);
}

void setEnv(){
    for(int i = 0; i < DIGIT; ++i){
        firstNumber[i] = -1;
        secondNumber[i] = -1;
        result[i] = -1;
    }
    result[DIGIT] = -1;

    puts("Unesite dva prirodna broja za racunanje sume!\n");
}

void sum(){
    int first = firstNumberSize - 1;
    int second = secondNUmberSize - 1;

    int carriage = 0;
    int resultCounter = 0;
    while(first >= 0 && second >= 0){
        int currentSum  = firstNumber[first--] + secondNumber[second--] + carriage;
        result[resultCounter++] = currentSum  % 10;
        carriage = currentSum  / 10;
    }

    while(first >= 0){
        int currentSum = firstNumber[first--] + carriage;
        result[resultCounter++] = currentSum % 10;
        carriage = currentSum / 10;
    }

    while(second >= 0){
        int currentSum = secondNumber[second--] + carriage;
        result[resultCounter++] = currentSum % 10;
        carriage = currentSum / 10;
    }

    if(carriage){
        result[resultCounter++] = carriage;
    }

    for(int i = resultCounter - 1; i >= 0; --i){
        putch((unsigned char)('0' + result[i]));
    }
}