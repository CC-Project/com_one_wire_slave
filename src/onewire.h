/**
*	Functions definitions for the One Wire bus
**/

#ifndef ONEWIRE_H_
#define ONEWIRE_H_
	#include "../config.h"
	#include "../../lib_data/data.h"
	#include <avr/io.h>
	#include <util/delay.h>

	#define T 60 // Standard time slot

	void bus_low(void);
	void bus_release(void);
	uint8_t bus_read(void);
#endif
