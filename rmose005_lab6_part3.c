/*	Author: Royce Moses
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  VIDEO: https://youtu.be/VhzbHQjtJMA
 * 
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

/* Insert initializations */
unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;
unsigned char i = 0x00;

/* Enumerate states */
enum SM1_STATES { SM1_Init, SM1_Wait, SM1_Inc, SM1_Dec, SM1_Reset } SM1_STATE;

/* Insert SM Functions */
void Tick_Count() { 
    switch (SM1_STATE) { 
        case SM1_Init:
            SM1_STATE = SM1_Wait;
            tmpB = 0x07; // C = 7
            break;
        case SM1_Wait:
            if (tmpA == 0x00) {
                SM1_STATE = SM1_Wait;
            }
            else if (tmpA == 0x01) {
                if (tmpB < 9) {
                    SM1_STATE = SM1_Inc;
                    tmpB = tmpB + 1;
                }
                else {
                    SM1_STATE = SM1_Wait;
                }
            }
            else if (tmpA == 0x02) {
                if (tmpB > 0) {
                    SM1_STATE = SM1_Dec;
                    tmpB = tmpB - 1;
                }
                else {
                    SM1_STATE = SM1_Wait;
                }
            }
            else {
                SM1_STATE = SM1_Reset;
                tmpB = 0;
            }
            break;
        case SM1_Inc:
            if (tmpA == 0x01) {
                SM1_STATE = SM1_Inc;
                if (i < 10) {
                    ++i;
                }
                else {
                    if (tmpB < 9)
                        tmpB = tmpB + 1;
                    i = 0;
                }
            }
            else if (tmpA == 0x03) {
                SM1_STATE = SM1_Reset;
                tmpB = 0;
                i = 0;
            }
            else {
                SM1_STATE = SM1_Wait;
                i = 0;
            }
            break;
        case SM1_Dec:
            if (tmpA == 0x02) {
                SM1_STATE = SM1_Dec;
                if (i < 10) {
                    ++i;
                }
                else {
                    if (tmpB > 0)
                        tmpB = tmpB - 1;
                    i = 0;
                }
            }
            else if (tmpA == 0x03) {
                SM1_STATE = SM1_Reset;
                tmpB = 0;
                i = 0;
            }
            else {
                SM1_STATE = SM1_Wait;
                i = 0;
            }
            break;
        case SM1_Reset:
            if (tmpA == 0x03) {
                SM1_STATE = SM1_Reset;
            }
            else {
                SM1_STATE = SM1_Wait;
            }
            break;
        default:
            SM1_STATE = SM1_Init;
            break;
    }
}


int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initalize to 1s.
        DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initalize to 0s.

    /* Insert Timer initializations */
    unsigned long M = 100;
    TimerSet(M);
    TimerOn(); 

    /* Insert your solution below */
    SM1_STATE = SM1_Init;
    while (1) {
        tmpA = ~PINA;
        Tick_Count();      // Execute one SM tick
        PORTB = tmpB;
        while (!TimerFlag); // Wait for M miliseconds
        TimerFlag = 0;
    }
    return 1;
}