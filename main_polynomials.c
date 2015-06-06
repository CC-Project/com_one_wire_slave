/*
 * Main file.
 */

#define N 7
#include "../code_polynomials/config_polynomials.h"
#include "../code_polynomials/src/poly.h"
#include "config.h"
#include "src/onewire.h"
#include "../lib_data/data.h"
#include "../lib_uart/uart.h"
#include <avr/interrupt.h>
#include <stdio.h>

volatile uint32_t i;
uint8_t j;
struct Data* data;
Poly generator;
Poly* syndrome;

int main(void)
{
	// Init the lib
	poly_init_generator();
	make_syndrome();
	
	i = 0;
	j = 0;
    data = data_generate(N);


	#ifdef DEBUG
		// Setup led for debugging
        //LED_DDR |= LED_PIN_MASK;
        //LED_PORT &=~LED_PIN_MASK;
	#endif
	uart_init(BAUD_RATE);

    // Pin change interrupt (INT1 pin)
    EICRA |= (1 << ISC11); // Falling edge
    EIMSK |= (1 << INT1); // Enable Interruption
    EIFR |= (1 << INTF1);

	char str[5];
	// Enable interrupts.
	sei();

    // Main loop
	for(;;)
	{
	    if(i == 7)
        {
			i = 0;
			Poly deco = poly_decode(data);
			data_show(deco);
			data_free(deco);
        }
	}
}



ISR (INT1_vect) // Falling edge detected
{
    _delay_us(T/4);
    if(bus_read())
    {
		data_set(i, 1, data);
    }
    else
    {
	    data_set(i, 0, data);
    }
	i += 1;
}
