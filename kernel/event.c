#include "stdint.h"
#include "stdbool.h"

#include "stdio.h"
#include "event.h"


static uint32_t sEventFlag;

void Kernel_event_flag_init(void)
{
    sEventFlag = 0;   //event flag 초기화
}

void Kernel_event_flag_set(KernelEventFlag_t event)
{
    sEventFlag |= (uint32_t)event;    //event flag를 1로 setting 한다.
}

void Kernel_event_flag_clear(KernelEventFlag_t event)
{
    sEventFlag &= ~((uint32_t)event); //event flag를 0으로 clear 한다.
}

bool Kernel_event_flag_check(KernelEventFlag_t event)
{
    if (sEventFlag & (uint32_t)event)  //event를 Check하여 값이 있으면
    {
        Kernel_event_flag_clear(event);  //event를 clear 한 후
        return true;                     //true return한다.
    }
    return false; //event가 없으므로 false를 return한다.
}
