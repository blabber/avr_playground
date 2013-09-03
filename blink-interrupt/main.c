/**
 * Simple timer interrupt example.
 * Written for ATtiny2313A
 */

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED1	(1<<PD5)
#define LED2	(1<<PD6)

#define TIMER_PRESCALE_8 (1<<CS11)

ISR(TIMER1_OVF_vect)
{
	PORTD ^= LED1 | LED2;
}

ISR(TIMER1_COMPA_vect)
{
	PORTD ^= LED2;
}

int
main(void)
{
	/* outputs */
	DDRD |= LED1 | LED2;

	/* set timer */
	TCCR1B |= TIMER_PRESCALE_8;

	OCR1A = (65535 / 2);

	/* enable interrupts */
	TIMSK |= (1<<TOIE1) | (1<<OCIE1A);
	sei();

	for (;;);

	/* NOTREACHED */
	return (0);
}
