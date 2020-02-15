#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

#include "stdlib.h"

static void Hw_init(void);
static void printf_test(void);
static void Timer_test(void);


int main(void)
{
    Hw_init();

    uint32_t length;
    putstr("Hello world!\n");

    printf_test();
    Timer_test();
    //length = getstr();


   while(true);
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
    Hal_timer_init();
}

static void printf_test(void)
{
    char *str = "test str";
    int i = 5;
    uint32_t* sysctrl0 = (uint32_t*)0x10001028;

    debug_printf("%s\n", "hello printf");
    debug_printf("output string pointer: %s\n",str);
    debug_printf("%u = 5\n",i);
    debug_printf("dec= %u hex = %x \n", 0xff, 0xff);
    debug_printf("sysctrl value = %x\n", *sysctrl0);
}

static void Timer_test(void)
{
    while(true)
    {
        debug_printf("current count : %u\n", Hal_timer_get_1ms_counter());
        delay(1000);
    }
}