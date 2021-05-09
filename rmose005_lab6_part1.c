/*	Author: Royce Moses
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  
 *  VIDEO: https://youtu.be/Iyf3-p3HoUw
 * 
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

/* Insert tmp initializations */
// unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;

enum SM1_STATES { SM1_Init, SM1_PB0_On, SM1_PB1_On, SM1_PB2_On } SM1_STATE;
void Tick_Cycle() {
    switch(SM1_STATE) {
        case SM1_Init:
            SM1_STATE = SM1_PB0_On;
            break;
        case SM1_PB0_On:
            SM1_STATE = SM1_PB1_On;
            break;
        case SM1_PB1_On:
            SM1_STATE = SM1_PB2_On;
            break;
        case SM1_PB2_On:
            SM1_STATE = SM1_PB0_On;
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
    unsigned long M = 1000;
    TimerSet(M);
    TimerOn();

    /* Insert your solution below */
    SM1_STATE = SM1_Init; // initial state
    
    while (1) {
        // tmpA = PINA;
        Tick_Cycle();      // Execute one SM tick
        PORTB = tmpB;
        while (!TimerFlag); // Wait for M miliseconds
        TimerFlag = 0;
    }
    return 1;
}
