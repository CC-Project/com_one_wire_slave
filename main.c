/*
 * Main file.
 */
#define DATA_SIZE 32
#define DATA
#include "config.h"
#include "src/onewire.h"
#include "../lib_data/data.h"
#include "../lib_uart/uart.h"
#include <avr/.h>

uint32_t nb_data;
struct Data* data;

int main(void)interrupt
{
    data = data_generate(DATA_SIZE);
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
            data_show(data);
            //decode(data)
        }
        _delay_ms(3000);
	}
}


ISR (INT1_vect) // Falling edge detected
{
    _delay_us(T/4);
    if(bus_read())
    {
        if(nb_data < 32)
        {
            data_set(nb_data, 1, data);
        }
        nb_data += 1;
    }
    else
    {
        if(nb_data < 32)
        {
            data_set(nb_data, 0, data);
        }
        nb_data += 1;
    }
}
