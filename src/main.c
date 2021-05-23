#define F_CPU 7372800UL

#include <stdbool.h>

#include <avr/io.h>
#include <util/delay.h>
#include "7seg.h"

#define LETTER_0 PORTA
#define LETTER_1 PORTC
#define LETTER_2 PORTB

int main() {

    DDRA = 0x7f;
    DDRB = 0x7f;
    DDRC = 0x7f;


    uint8_t n = 0;

    while(true) {
        LETTER_0 = SEGMENT_MASK[n + 0];
        LETTER_1 = SEGMENT_MASK[n + 1];
        LETTER_2 = SEGMENT_MASK[n + 2];

        n++;
        if(n > 16) n = 0;

        _delay_ms(400);
    }
}