#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

#include "Kernel.h"

#include "main.h"

int main(void)
{
    Hw_init();

    uint32_t length;
    putstr("Hello world!\n");
    Kernel_task_init();
    Kernel_init();

    //printf_test();
    //Timer_test();
    //length = getstr();


   while(true);
}

static void Kernel_init(void)
{
    uint32_t taskId;

    taskId = Kernel_task_create(User_task0,1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task0 creation fail\n");
    }

    taskId = Kernel_task_create(User_task1,1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task1 creation fail\n");
    }

    taskId = Kernel_task_create(User_task2,1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task2 creation fail\n");
    }

    taskId = Kernel_task_create(User_task3,1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task3 creation fail\n");
    }

    taskId = Kernel_task_create(User_task4,1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task4 creation fail\n");
    }


    Kernel_start();

}



void User_task0(void)
{
    uint32_t local = 0;

    while(true)
    {
        debug_printf("User Task #0\n", &local);
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn|KernelEventFlag_CmdOut);
        switch(handle_event)
        {
            case KernelEventFlag_UartIn:
            debug_printf("Uart Event Handle \n");
            Kernel_send_events(KernelEventFlag_CmdOut);
            break;
            /*
            case KernelEventFlag_CmdOut:
            debug_printf("Cmdout Event Handle \n");
            break;
            */


        }
        
        delay(1000);
        Kernel_yield();
    }
}

void User_task1(void)
{
      int32_t local = 0;

    while(true)
    {
        debug_printf("User Task #1\n", &local);
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_CmdOut);
        switch(handle_event)
        {
            case KernelEventFlag_CmdOut:
            debug_printf("Cmd Event Handle \n");
            break;
        }
        
        delay(1000);
        Kernel_yield();
    }

}

void User_task2(void)
{
    int32_t local = 0;

    while(true)
    {
        debug_printf("User Task #2\n", &local);
        delay(1000);
        Kernel_yield();
    }

}


void User_task3(void)
{
    int32_t local = 0;

    while(true)
    {
        debug_printf("User Task #3\n", &local);
        delay(1000);
        Kernel_yield();
    }

}


void User_task4(void)
{
    int32_t local = 0;

    while(true)
    {
        debug_printf("User Task #4\n", &local);
        delay(1000);
        Kernel_yield();
    }

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