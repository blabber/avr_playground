/**
 * Simple external interrupt example.
 * Written for ATtiny2313A
 */

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#define BUTTON	(1<<PD2)
#define LED	(1<<PD5)

#define INT0_FALLING_EDGE	(1<<ISC11)

volatile uint8_t flag;

ISR(INT0_vect)
{
	flag = 1;
}

int main(void)
{
	/* outputs */
	DDRD |= LED;

	/* enable pullups */
	PORTD |= BUTTON;

	/* configure interrupt */
	MCUCR |= INT0_FALLING_EDGE;

	/* enable interrupts */
	GIMSK |= (1<<INT0);
	sei();

	for (;;) {
		if (flag) {
			/*
			 * Disable interrupts, toggle the LED and sleep some
			 * milliseconds to debounce. After that clear the
			 * interrupt flag in case anouther interrupt has been
			 * triggered (by bouncing). Finally enable interrupts.
			 */

			cli();

			flag = 0;
			PORTD ^= LED;

			_delay_ms(250);
			EIFR |= INTF0;

			sei();
		}
	}

	/* NOTREACHED */
	return (0);
}
