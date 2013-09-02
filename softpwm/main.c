/**
 * Simple softpwm example.
 * Written for ATtiny2313A
 */

#define F_CPU	8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define LED1	(1<<PD5)
#define LED2	(1<<PD6)

#define TIMER_NOPRESCALER	(1<<CS10)

#define PWM_STEPS		(64)
#define PWM_LEN			(65536/PWM_STEPS)

volatile uint8_t pwm1;
volatile uint8_t pwm2;

ISR(TIMER1_COMPA_vect)
{
	static uint8_t pwmcnt = 0;

	if (pwm1 <= pwmcnt)
		PORTD &= ~(LED1);
	else
		PORTD |= LED1;

	if (pwm2 <= pwmcnt)
		PORTD &= ~(LED2);
	else
		PORTD |= LED2;

	OCR1A += PWM_LEN;	

	if (++pwmcnt == PWM_STEPS)
		pwmcnt = 0;

	TIFR = (1<<OCF1A);
}

int
main(void)
{
	uint8_t pwmdir1 = 1;
	uint8_t pwmdir2 = -1;

	DDRD |= LED1 | LED2;
	TCCR1B |= TIMER_NOPRESCALER;
	TIMSK |= (1<<OCIE1A);

	sei();

	pwm1 = 0;
	pwm2 = PWM_STEPS;

	for (;;)
	{
		if (pwm1 == PWM_STEPS - 1)
			pwmdir1 = -1;
		else if (pwm1 == 0)
			pwmdir1 = 1;
		pwm1 += pwmdir1;

		if (pwm2 == PWM_STEPS - 1)
			pwmdir2 = -1;
		else if (pwm2 == 0)
			pwmdir2 = 1;
		pwm2 += pwmdir2;

		_delay_ms(32);
	}

	/* NOTREACHED */
	return (0);
}
