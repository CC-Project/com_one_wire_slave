/*
 * Main file.
 */

//Import all modules and libs
#include "../lib_data/data.h"
#include "../lib_utilities/utilities.h"

#include "src/hamming.h"
#include "src/matrix.h"

// Import configuration
#include "hamming_config.h"
#include "config.h"
#include "src/onewire.h"
#include "../lib_data/data.h"
#include "../lib_uart/uart.h"
#include <avr/interrupt.h>

uint32_t nb_data;
struct Data* _data;

int main(void)
{
    _data = data_generate(32);
    nb_data = 0;

	#ifdef DEBUG
		//Setup led for debugging
        LED_DDR |= LED_PIN_MASK;
        LED_PORT &= ~LED_PIN_MASK;
	#endif
	uart_init(BAUD_RATE);

    // Pin change interrupt (INT1 pin)
    EICRA |= (1 << ISC11); // Falling edge
    EIMSK |= (1 << INT1); // Enable Interruption
    EIFR |= (1 << INTF1);

	// Enable interrupts.
	sei();

    //Main loop
	for(;;)
	{
	    if(nb_data == 32)
        {
            nb_data = 0;
            data_show(_data);
        }
        _delay_ms(3000);
	}
}


ISR (INT1_vect) // Falling edge detected
{
    _delay_us(T/4);
    if(bus_read())
    {
        if(nb_data < 31)
        {
            data_set(nb_data, 1, _data);
        }
        nb_data += 1;
    }
    else
    {
        if(nb_data < 31)
        {
            data_set(nb_data, 0, _data);
        }
        nb_data += 1;
    }
}
