/**
 * Simple fast pwm, controlled by buttons.
 * Written for ATtiny2313A
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define BUTTON1	(1<<PD2)
#define BUTTON2	(1<<PD3)
#define LED	(1<<PD5)
#define PWM_LED	(1<<PB3)

#define PWM_NONINVERTED (1<<COM1A1)
#define PWM_FAST_10	(1<<WGM12) | (1<<WGM11) | (1<<WGM10)
#define PWM_BOTTOM	0
#define PWM_TOP		1023

#define TIMER_NO_PRESCALE	(1<<CS10)

volatile uint16_t pwmval = 0;

int
main(void)
{
	/* outputs */
	DDRD |= LED;
	DDRB |= PWM_LED;

	/* internal pullups */
	PORTD |= BUTTON1 | BUTTON2;

	/* set timer / pwm */
	TCCR1A |= PWM_NONINVERTED | PWM_FAST_10;
	TCCR1B |= TIMER_NO_PRESCALE;

	OCR1A = pwmval;

	for (;;) {
		if (!(PIND & BUTTON1)) {
			/* toggle led */
			PORTD ^= LED;

			if (pwmval < (PWM_BOTTOM + 10))
				pwmval = PWM_BOTTOM;
			else
				pwmval -= 10;

			OCR1A = pwmval;

			/* prinmitive debounce */
			_delay_ms(250);
		}

		if (!(PIND & BUTTON2)) {
			/* toggle led */
			PORTD ^= LED;

			if (pwmval > (PWM_TOP - 10))
				pwmval = PWM_TOP;
			else
				pwmval += 10;
			
			OCR1A = pwmval;

			/* prinmitive debounce */
			_delay_ms(250);
		}
	}

	/* NOTREACHED */
	return (0);
}
