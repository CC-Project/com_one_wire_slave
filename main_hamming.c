/*
 * Main file.
 */

//Import all modules and libs
#include "../lib_data/data.h"
#include "../lib_utilities/utilities.h"

#include "../code_matrix/src/hamming.h"
#include "../code_matrix/src/matrix.h"

// Import configuration
//#include "../code_matrix/hamming_config.h"
#include "config.h"
#include "src/onewire.h"
#include "../lib_data/data.h"
#include "../lib_uart/uart.h"
#include <avr/interrupt.h>

uint32_t i;
uint32_t nb_data;
struct Matrix* data;

int main(void)
{
    struct Hamming_config* conf = hamming_generate_config();
    nb_data = 0;
    data = matrix_generate(N, 1);
    i = 1;

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
        if(i == N)
        {
            i = 1;
            //struct Matrix* tmp = hamming_correction(data, conf);
            //struct Matrix* decoded = hamming_decode(data, conf);
            data_show(data->data);
            //matrix_free(tmp);
            //matrix_free(decoded);
            for(;;)
            {

            }
        }
	}
}


ISR (INT1_vect) // Falling edge detected
{
    _delay_us(T/3);
    if(bus_read())
    {
        if(i <= 7)
            matrix_set(data, i, 1, 1);

        i += 1;
    }
    else
    {

        if(i <= 7)
            matrix_set(data, i, 1, 0);
        i+=1;
    }
}
