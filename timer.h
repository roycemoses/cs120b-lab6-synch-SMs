#ifndef ___TIMER_H___
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1 ms ticks

void TimerOn() {
    // AVR timer/counter controller register TCCR1
    TCCR1B = 0x0B;  // bit 3 = 0: CTC mode (clear timer on compare)
                    // bit2bit1bit0=011: pre-scaler /64
                    // So, 8 MHz clock or 8,000,000 / 64 = 125,000 ticks per second
                    // Thus, TCNT1 register will count at 125,000 ticks per second

    // AVR output compare register OCR1A.
    OCR1A = 125;    // Timer interrupt will be generated when TCNT1 == OCR1A
                    // We want a 1 ms tick. 0.001 s * 125,000 ticks / second = 125
                    // So when TCNT1 register equals 125,
                    // 1 ms has passed. Thus, we compare to 125.

    // AVR timer interrupt mask register
    TIMSK1 = 0x02;  // bit1: OCIE1A -- enables compare match interrupt

    // Initialize AVR counter
    TCNT1 = 0;

    _avr_timer_cntcurr = _avr_timer_M; // TimerISR will be called ever _avr_timer_cntcurr miliseconds

    // Enable global interrupts
    SREG |= 0x80; // 0x80: 1000 0000

}

// The "enable global interrupts" line is SREG |= 0x80, not SREG |- 0x80;

void TimerOff() {
    TCCR1B = 0x00; // bit3bit1bit0 = 000: timer off
}

void TimerISR() {
    TimerFlag = 1;
}


// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
    // CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
    _avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
    if (_avr_timer_cntcurr == 0) {
        TimerISR(); // Call the ISR that the user uses
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

#endif // ___TIMER_H___