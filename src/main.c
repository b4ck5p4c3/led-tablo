#define F_CPU 7372800UL

#include <stdbool.h>

#include <avr/io.h>
#include <avr/delay.h>

int main() {
    DDRB |= 1 << PB0;

    while(true) {
        PORTB ^= 1 << PB0;
        _delay_ms(1000);
    }
}