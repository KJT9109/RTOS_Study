#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

#include "Kernel.h"

#include "main.h"
#include "msg.h"



int main(void)
{
    Hw_init();

    uint32_t length;
    putstr("Hello world!\n");
    printf_test();
    Kernel_task_init();
    Kernel_mutex_init();
    Kernel_init();
    //Timer_test();
    //length = getstr();


   while(true);
}

static void Kernel_init(void)
{
    uint32_t taskId;
    Kernel_event_flag_init();
    Kernel_msgQ_init();
    Kernel_sem_init(1);
  

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
    uint8_t  cmdBuf[16];
    uint8_t  recvBuf[16];
    
    uint8_t recv_length = 0;
    uint8_t recv_index = 0;
    uint8_t fail_index = 0;

    while(true)
    {
        debug_printf("User Task #0\n");
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn|KernelEventFlag_MutexLock);
        switch(handle_event)
        {
            case KernelEventFlag_UartIn:
                debug_printf("Uart Event Handle \n");
                recv_length = MsgQ_recv_length(KernelMsgQ_Task0);
                Kernel_recv_msg(KernelMsgQ_Task0,recvBuf,recv_length);

                if ((recvBuf[recv_length-1] == '\r'))
                {   
                    recv_index = Buffer_push_msg(&recv_index,&recv_length,recvBuf,cmdBuf);
                    Kernel_send_msg(KernelMsgQ_Task1, &recv_index, 1);
                    Kernel_send_msg(KernelMsgQ_Task1, cmdBuf, recv_index);
                    Kernel_send_events(KernelEventFlag_CmdOut);
                    memclr(cmdBuf, 16);
                    recv_index = 0;
                   
                }
                else
                {
                    fail_index = recv_index;
                    recv_index = Buffer_push_msg(&recv_index,&recv_length,recvBuf,cmdBuf);
                    if(recv_index == fail_index)
                    {
                        debug_printf("data save fail.\n");
                        
                    }
                    else
                    {
                        debug_printf("data saved \n");
                    }
                    
                }

            break;

            case KernelEventFlag_MutexLock:
                Mutex_critical_section(0,0);
            
            break;

        }
        
        delay(1000);
        Kernel_yield();
    }
}

void User_task1(void)
{

    uint8_t cmdlen = 0;
    uint8_t cmd[16] = {0};


    while(true)
    {
        debug_printf("User Task #1\n");
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_CmdOut);
        switch(handle_event)
        {
            case KernelEventFlag_CmdOut:
                memclr(cmd, 16);
                Kernel_recv_msg(KernelMsgQ_Task1, &cmdlen, 1);
                Kernel_recv_msg(KernelMsgQ_Task1, cmd, cmdlen);

                debug_printf("receive Message: %s\n",cmd);
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
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_Semaphore);
        switch(handle_event)
        {
            case KernelEventFlag_Semaphore:
                debug_printf("event semaphore\n");
                Test_critical_section(2,2);
            break;
        }
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
        //Test_critical_section(3,3);
        Mutex_critical_section(3,3);
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
        Mutex_critical_section(4,4);
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

uint8_t Buffer_push_msg(uint8_t* index, uint8_t* length, uint8_t* sendBuf, uint8_t* recvBuf)
{
    uint8_t sIndex =  *index;
    uint8_t sLength = *length;

    if((sIndex + sLength) < 16)
    {
        for(int i = sIndex; i < (sIndex + sLength); i++)
        {   
            recvBuf[i] = *(sendBuf +i - sIndex);
        
        }

        sIndex += sLength;

        return sIndex;
    }

    else
    {
        return sIndex;
    }
}


static uint32_t shared_value;
static void Test_critical_section(uint32_t p, uint32_t taskId)
{
    Kernel_lock_sem();

    debug_printf("section Task #%u Send=%u\n", taskId, p);
    shared_value = p;
    Kernel_yield();
    delay(1000);
    debug_printf("section 2nd Task #%u Shared Value=%u\n", taskId, shared_value);

    Kernel_unlock_sem();
}

static void Mutex_critical_section(uint32_t p, uint32_t taskId)
{
    Kernel_lock_mutex();

    debug_printf("Mutex 1st Task #%u Send=%u\n", taskId, p);
    shared_value = p;
    Kernel_yield();
    delay(1000);
    debug_printf("Mutex 2nd Task #%u Shared Value=%u\n", taskId, shared_value);

    Kernel_unlock_mutex();
  
}

