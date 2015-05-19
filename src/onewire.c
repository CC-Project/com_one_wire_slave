/**
*	Functions for the One Wire bus
*	Officials timing taken from http://www.maximintegrated.com/en/app-notes/index.mvp/id/126
*	These funcs are called through interrupts
**/

#include "../../lib_uart/uart.h"
#include "onewire.h"

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
