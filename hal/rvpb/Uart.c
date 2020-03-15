#include "stdint.h"
#include "stdbool.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"

#include "Kernel.h"

extern volatile PL011_t* Uart;
static void interrupt_handler(void);

void Hal_uart_init(void)
{
    //Enable UART
    Uart->uartcr.bits.UARTEN = 0;
    Uart->uartcr.bits.TXE = 1;
    Uart->uartcr.bits.RXE = 1;
    Uart->uartcr.bits.UARTEN = 1;

     Uart->uartimsc.bits.RXIM = 1;

    Hal_interrupt_enable(UART_INTERRUPT0);
    Hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}

void Hal_uart_put_char(uint8_t ch)
{
    while(Uart->uartfr.bits.TXFF);
    Uart->uartdr.all = (ch & 0xFF);
}

uint8_t Hal_uart_get_char(void)
{
    uint32_t data;

    while(Uart->uartfr.bits.RXFE);
    
    data = Uart->uartdr.all;

    if(data&0xffffff00)
    {
        Uart->uartrsr.all = 0xff;
        return 0;
    }

    return (uint8_t)(data & 0xff);
}

static void interrupt_handler(void)
{
    uint8_t ch = Hal_uart_get_char();
    Hal_uart_put_char(ch);
    Hal_uart_put_char('\n');
    
    Kernel_send_msg(KernelMsgQ_Task0,&ch,1);
    Kernel_send_events(KernelEventFlag_UartIn);

}
   