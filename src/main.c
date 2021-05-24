#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "yaMBSiavr.h"
#include "7seg.h"

#define PORT_DIGIT_MASK 0x7f
#define PORT_RAW_MASK 0x80

#define LETTER_0 PORTA
#define LETTER_1 PORTC
#define LETTER_2 PORTB

#define ADDRESS 0x08

uint16_t holding_registers[4] = {0};
volatile uint8_t letters[3] = {0};

void setup_modbus_timer() {
    TCCR0 |= (1 << CS02) | (1 << WGM01);
    TIMSK |= 1 << OCIE0;
    OCR0 = 29;
}

void setup_dimm_timer() {
    TIMSK |= (1 << TOIE1) | (1 << OCF1B);
    TCCR1B |= (1<< CS10) ;
}

void clear_letters() {
    LETTER_0 &= ~PORT_DIGIT_MASK;
    LETTER_1 &= ~PORT_DIGIT_MASK;
    LETTER_2 &= ~PORT_DIGIT_MASK;
}

void show_letters() {
    LETTER_0 |= letters[0] & PORT_DIGIT_MASK;
    LETTER_1 |= letters[1] & PORT_DIGIT_MASK;
    LETTER_2 |= letters[2] & PORT_DIGIT_MASK;
}

ISR(TIMER0_COMP_vect) {
   modbusTickTimer();
}

ISR(TIMER1_COMPB_vect) {
    clear_letters();
}

ISR(TIMER1_OVF_vect) {
    show_letters();
}

void update_duty_cycle() {
    uint16_t duty = holding_registers[3];
    if (duty == 0xffff) duty = 0xfffe;
    if (duty == 0) {
        letters[0] = 0;
        letters[1] = 0;
        letters[2] = 0;
    } else if (duty < 3) {
        duty = 3;
    }
    OCR1B = duty;
}

void update_letters(void) {
    for (int i = 0; i < 3; ++i) {
        uint8_t port_value = 0;
        if (holding_registers[i] & PORT_RAW_MASK) {
            letters[i] = holding_registers[i] & PORT_DIGIT_MASK;
        } else {
            letters[i] = SEGMENT_MASK[holding_registers[i] & 0x0f];
        }
    }
}

void modbus_get(void) {
    if (modbusGetBusState() & (1 << ReceiveCompleted)) {
        switch(rxbuffer[1]) {
            case fcPresetSingleRegister:
            case fcPresetMultipleRegisters:
                modbusExchangeRegisters(holding_registers, 0, 4);
                update_letters();
                update_duty_cycle();
                break;
            default:
                modbusSendException(ecIllegalFunction);
        }
    }
}

int main() {
    DDRA = 0x7f;
    DDRB = 0x7f;
    DDRC = 0x7f;

    holding_registers[0] = 0xff;
    holding_registers[1] = 0xff;
    holding_registers[2] = 0xff;
    holding_registers[3] = 0xfffe;

    setup_modbus_timer();
    setup_dimm_timer();

    update_letters();
    update_duty_cycle();

    modbusSetAddress(ADDRESS);
    modbusInit();
    sei();

    while(true) {
        modbus_get();
    }
}