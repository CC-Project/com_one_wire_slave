/**
*	Functions for the One Wire bus
*	Officials timing taken from http://www.maximintegrated.com/en/app-notes/index.mvp/id/126
*	These funcs are called through interrupts
**/

#include "../../lib_uart/uart.h"
#include "onewire.h"

/*	Utility functions	*/
void bus_low(void)
{
	OW_BUS_DDR  |= OW_BUS_PIN_MASK;
	OW_BUS_PORT &= ~OW_BUS_PIN_MASK;
}

void bus_release(void)
{
	//OW_BUS_PORT |= OW_BUS_PIN_MASK;		// Enable the pull up resistor, or set the bus high
	OW_BUS_DDR  &= ~OW_BUS_PIN_MASK;	// Make the pin as input
}

uint8_t bus_read(void)
{
	return (OW_BUS_PIN & OW_BUS_PIN_MASK);
}

/*	One Wire functions	*/
void control(uint32_t* nb_data, struct Data* _data)
{
    /*
        Bus low for T/6 = 10µs.
        Assume delay is between 8µs and 12µs (+- 2µs)
    */
    if ( 128 <= TCNT1 && TCNT1 <= 192) //Bit 1
    {
        //data_set(*nb_data, 1, _data);
        if(*nb_data < 31)
            *nb_data += 1;
    }
    else if (448 <= TCNT1 && TCNT1 <= 512) //Bit 0
    {
        //data_set(*nb_data, 0, _data);
        if(*nb_data < 31)
            *nb_data += 1;
    }
    else if (TCNT1 >= 7968) // >= 480µs
    {
        ow_reset();
    }
}

void ow_reset(void)
{
    _delay_us(T);
	bus_low();
	_delay_us(4*T);
	bus_release();
	_delay_us(3*T);
}
