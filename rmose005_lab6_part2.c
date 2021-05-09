/*	Author: Royce Moses
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  
 *  VIDEO: https://youtu.be/v9JkCmBzFWA
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
enum SM1_STATES { SM1_Init, SM1_PB0_On, SM1_PB1_On, SM1_PB2_On, SM1_PB0_AlwaysOn_Wait, SM1_PB0_AlwaysOn, SM1_PB1_AlwaysOn_Wait, 
SM1_PB1_AlwaysOn, SM1_PB2_AlwaysOn_Wait, SM1_PB2_AlwaysOn } SM1_STATE;

/* Insert SM Functions */
void Tick_Cycle() {
    switch(SM1_STATE) {
        case SM1_Init:
            if (tmpA)
                SM1_STATE = SM1_Init;
            else
                SM1_STATE = SM1_PB0_On;
            break;
        case SM1_PB0_On:
            if (tmpA) {
                SM1_STATE = SM1_PB0_AlwaysOn_Wait;
                i = 0;
            }
            else if (i < 30) {
                ++i;
            }
            else {
                SM1_STATE = SM1_PB1_On;
                i = 0;
            }
            break;
        case SM1_PB1_On:
            if (tmpA) {
                SM1_STATE = SM1_PB1_AlwaysOn_Wait;
                i = 0;
            }
            else if (i < 30) {
                ++i;
            }
            else {
                SM1_STATE = SM1_PB2_On;
                i = 0;
            }
            break;
        case SM1_PB2_On:
            if (tmpA) {
                SM1_STATE = SM1_PB2_AlwaysOn_Wait;
                i = 0;
            }
            else if (i < 30) {
                ++i;
            }
            else {
                SM1_STATE = SM1_PB0_On;
                i = 0;
            }
            break;
        case SM1_PB0_AlwaysOn_Wait:
            if (tmpA)
                SM1_STATE = SM1_PB0_AlwaysOn_Wait;
            else
                SM1_STATE = SM1_PB0_AlwaysOn;
            break;
        case SM1_PB0_AlwaysOn:
            if (tmpA)
                SM1_STATE = SM1_Init;
            else
                SM1_STATE = SM1_PB0_AlwaysOn;
            break;
        case SM1_PB1_AlwaysOn_Wait:
            if (tmpA)
                SM1_STATE = SM1_PB1_AlwaysOn_Wait;
            else
                SM1_STATE = SM1_PB1_AlwaysOn;
            break;
        case SM1_PB1_AlwaysOn:
            if (tmpA)
                SM1_STATE = SM1_Init;
            else
                SM1_STATE = SM1_PB1_AlwaysOn;
            break;
        case SM1_PB2_AlwaysOn_Wait:
            if (tmpA)
                SM1_STATE = SM1_PB2_AlwaysOn_Wait;
            else
                SM1_STATE = SM1_PB2_AlwaysOn;
            break;
        case SM1_PB2_AlwaysOn:
            if (tmpA)
                SM1_STATE = SM1_Init;
            else
                SM1_STATE = SM1_PB2_AlwaysOn;
            break;
        default:
            SM1_STATE = SM1_Init;
            break;
    }
    switch(SM1_STATE) {
        case SM1_PB0_On:
            tmpB = 0x01;
            break;
        case SM1_PB1_On:
            tmpB = 0x02;
            break;
        case SM1_PB2_On:
            tmpB = 0x04;
            break;
        case SM1_PB0_AlwaysOn_Wait:
            tmpB = 0x01;
            break;
        case SM1_PB0_AlwaysOn:
            tmpB = 0x01;
            break;
        case SM1_PB1_AlwaysOn_Wait:
            tmpB = 0x02;
            break;
        case SM1_PB1_AlwaysOn:
            tmpB = 0x02;
            break; 
        case SM1_PB2_AlwaysOn_Wait:
            tmpB = 0x04;
            break;
        case SM1_PB2_AlwaysOn:
            tmpB = 0x04;
            break; 
        default:
            tmpB = 0x01;
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initalize to 1s.
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initalize to 0s.

    /* Insert Timer initializations */
    unsigned long M = 10;
    TimerSet(M);
    TimerOn(); 

    /* Insert your solution below */
    SM1_STATE = SM1_Init; // initial state
    
    while (1) {
        tmpA = ~PINA;
        Tick_Cycle();      // Execute one SM tick
        PORTB = tmpB;
        while (!TimerFlag); // Wait for M miliseconds
        TimerFlag = 0;
    }
    return 1;
}
