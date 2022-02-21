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

int firstNumber[DIGIT];
int secondNumber[DIGIT];
int result[DIGIT + 1];

int firstNumberSize = 0;
int secondNumberSize = 0;

bool firstNumberNegative = false;
bool secondNumberNegative = false;

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
        if(symbol == '-' && counter == 0){
            if(enterClickNumber){
                secondNumberNegative = true;
            } else {
                firstNumberNegative = true;
            }
            putch(symbol);
        }else if((symbol >= '0') && (symbol <= '9') && (counter < DIGIT)){
            int number = symbol - '0';
            if(enterClickNumber){
                secondNumber[secondNumberSize++] = number;
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
                if((firstNumberNegative && !secondNumberNegative) || (!firstNumberNegative && secondNumberNegative)){
                    sub();
                } else {
                    sum();
                }
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
    int second = secondNumberSize - 1;

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

    if(firstNumberNegative && secondNumberNegative){
        putch('-');
    }
    for(int i = resultCounter - 1; i >= 0; --i){
        putch((unsigned char)('0' + result[i]));
    }
}

int isGreater(){
    if(firstNumberSize > secondNumberSize){
        return 1;
    } else if(secondNumberSize > firstNumberSize){
        return 2;
    } else {
        for(int i = firstNumberSize - 1; i >= 0; --i){
            if(firstNumber[i] > secondNumber[i]){
                return 1;
            } else if(secondNumber[i] > firstNumber[i]){
                return 2;
            }
        }
        return 0;
    }
}

void sub(){
    int greater = isGreater();
    if(greater == 0){
        putch('0');
        return;
    }

    int first = firstNumberSize - 1;
    int second = secondNumberSize - 1;

    int carriage = 0;
    int resultCounter = 0;

    while(first >= 0 && second >= 0){
        int currentSub;
        if(greater == 2){
            currentSub = secondNumber[second--] - firstNumber[first--] - carriage;
        } else {
            currentSub = firstNumber[first--] - secondNumber[second--] - carriage;
        }
        if(currentSub < 0){
            currentSub += 10;
            carriage = 1;
        } else {
            carriage = 0;
        }
        result[resultCounter++] = currentSub;
    }

    while(first >= 0){
        int currentSub = firstNumber[first--] - carriage;
        if(currentSub < 0){
            currentSub += 10;
            carriage = 1;
        } else {
            carriage = 0;
        }
        result[resultCounter++] = currentSub;
    }

    while(second >= 0){
        int currentSub = secondNumber[second--] - carriage;
        if(currentSub < 0){
            currentSub += 10;
            carriage = 1;
        } else {
            carriage = 0;
        }
        result[resultCounter++] = currentSub;
    }

    if((greater == 1 && firstNumberNegative) || (greater == 2 && secondNumberNegative)){
        putch('-');
    }

    for(int i = resultCounter - 1; i >= 0; --i){
        putch((unsigned char)('0' + result[i]));
    }
}