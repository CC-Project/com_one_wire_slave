/**
*	Configuration file for the One Wire bus
**/

#ifndef CONFIG_OW_S_H
#define CONFIG_OW_S_H
	#define DEBUG // Enable debugging
	#ifdef DEBUG
		#define BAUD_RATE 0x19
		#define LED_DDR DDRB
		#define LED_PORT PORTB
		#define LED_PIN_MASK _BV(5)
	#endif

	// CPU clock frequency
	#define F_CPU	16000000UL

	// Pin configuration. INT1 used.
	#define OW_BUS_DDR		DDRD
	#define OW_BUS_PIN 		PIND
	#define OW_BUS_PORT		PORTD
	#define OW_BUS_PIN_MASK	_BV(3)
#endif
