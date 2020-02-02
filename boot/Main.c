#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#include "HalUart.h"
#include "HalInterrupt.h"



static void Hw_init(void);
static void printf_test(void);

int main(void)
{
    Hw_init();

    uint32_t length;
    putstr("Hello world!\n");

    printf_test();
    //length = getstr();


   while(true);
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
    
}

static void printf_test(void)
{
    char *str = "test str";
    int i = 5;

    debug_printf("%s\n", "hello printf");
    debug_printf("output string pointer: %s\n",str);
    debug_printf("%u = 5\n",i);
    debug_printf("dec= %u hex = %x \n", 0xff, 0xff);
}