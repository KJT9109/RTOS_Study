#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

uint32_t putstr(const char* s)
{
	uint32_t c = 0;
	while(*s)
	{
		Hal_uart_put_char(*s++);

	}

	return c;
}


uint32_t getstr(void)
{
	uint32_t c = 0;
	uint8_t ch[100];
	while(1)
	{
		ch[c] = Hal_uart_get_char();
		Hal_uart_put_char(ch[c]);
		if(ch[c] !='\r')
		{
			c++;
		}
		else
		{
			ch[c] ='\n';
			Hal_uart_put_char(ch[c]);
			break;
		}
		
	
	}
	putstr("Receive: ");
	putstr(ch);

	return c;
}