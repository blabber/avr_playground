/**
 * Simple unidirectional (TX) software uart.
 * Written for ATtiny2313A.
 */

#define F_CPU 8000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define TX	(1<<PB0)

#define BAUD	(9600)
#define BITTIME_US	(1000000/BAUD)

void uart_putc(char c);
void uart_puts(char *s);
void delay_bittime(void);

int
main(void)
{
	DDRB |= TX;

	for (;;) {
		uart_puts("Das ist ein Test\r\n");
		_delay_ms(500);
	}

	/* NOTREACHED */
	return (0);
}

void
uart_putc(char c)
{
	PORTB &= ~(TX);
	_delay_us(BITTIME_US);

	for (uint8_t i = 0; i < 8; i++) {
		if (c & (1<<0))
			PORTB |= TX;
		else
			PORTB &= ~(TX);
		c = (c>>1);
		_delay_us(BITTIME_US);
	}

	PORTB |= TX;
	_delay_us(BITTIME_US);
}

void
uart_puts(char *s)
{
	while (*s != '\0')
		uart_putc(*(s++));
}
