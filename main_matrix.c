/*
 * Main file.
 */

#define N 7
#include "config.h"
#include "src/onewire.h"
#include "data.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include "hamming_config.h"
#include "matrix.h"
#include "hamming.h"


volatile uint32_t i;
uint8_t j;
struct Matrix* data;

int main(void)
{
	struct Hamming_config* conf = hamming_generate_config();
	i = 1;
	j = 0;
    data = matrix_generate(N,1);

	#ifdef DEBUG
		//Setup led for debugging
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

    //Main loop
	for(;;)
	{
		sprintf(str, "%d\n", freeRam());
		uart_tx_str(str);
	    if(i == 8)
        {
			struct Matrix* tmp = hamming_correction(data, conf);
			struct Matrix* deco = hamming_decode(tmp, conf);
			data_show(deco->data);
			matrix_free(tmp);
			matrix_free(deco);
			i = 1;
        }
	}
}


ISR (INT1_vect) // Falling edge detected
{
    _delay_us(T/4);
    if(bus_read())
    {
		matrix_set(data, i, 1, 1);
    }
    else
    {
	    matrix_set(data, i, 1, 0);
    }
	i += 1;
}
