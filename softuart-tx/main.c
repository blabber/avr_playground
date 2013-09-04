/**
 * Simple unidirectional (TX) software UART.
 * Written for ATtiny2313A.
 *
 * This implementation is blocking. As it is just implementing the TX side of
 * the UART this should be no problem as no polling is required. To push out
 * measurements or debug messages on a MCU without hardware UART this might be
 * good enough.
 *
 * For bidirectional communication a interrupt based implementation as described
 * by Atmel Applicaltion Note AVR304 should be preferred.
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
	PORTB |= TX;

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
