/*
 * Main file.
 */

#include "config.h"
#include "src/onewire.h"
#include "../lib_data/data.h"
#include "../lib_uart/uart.h"
#include <avr/interrupt.h>

uint32_t nb_data;
struct Data* _data;

void main_slave();

void main_slave()
{
    _data = data_generate(32);
    nb_data = 0;

	#ifdef DEBUG
		uart_init(BAUD_RATE);

		//Setup led for debugging
        LED_DDR |= LED_PIN_MASK;
        LED_PORT &= ~LED_PIN_MASK;
	#endif

    // Pin change interrupt (INT1 pin)
    EICRA |= (1 << ISC11); // Falling edge
    EIMSK |= (1 << INT1); // Enable Interruption
    EIFR |= (1 << INTF1);

	// Enable interrupts.
	sei();

    //Main loop
	for(;;)
	{
        char str[10];
        sprintf(str, "%d", nb_data);
        uart_tx_str(str);
        uart_newline();
        _delay_ms(3000);

	}
	return 1;
}


ISR (INT1_vect) // If bus state changes
{
    if(nb_data == 31)
        nb_data = 0;

    if(EICRA & (1 << ISC10)) // Rising edge
    {
        EICRA &= ~(1 << ISC10);
        TCCR1B &= ~(1 << CS00); // Stop timer
        control(&nb_data,_data);
    }
    else // Falling edge
    {
        EICRA |= (1 << ISC10);
		TCCR1B |= (1 << CS00); // Start timer (no prescale)
        TCNT1 = 0;
    }
}
