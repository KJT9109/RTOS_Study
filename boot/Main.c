#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

#include "stdlib.h"


#include "Kernel.h"

static void Hw_init(void);
static void printf_test(void);
static void Timer_test(void);

static void Kernel_init(void);

void User_task0(void);
void User_task1(void);
void User_task2(void);
void User_task3(void);
void User_task4(void);

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

    taskId = Kernel_task_create(User_task2,0);
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