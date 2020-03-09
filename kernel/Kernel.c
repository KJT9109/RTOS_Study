#include "stdint.h"
#include "stdbool.h"

#include "Kernel.h"

void Kernel_yield(void)
{
    Kernel_task_scheduler();
}


void Kernel_start(void)
{
    Kernel_task_start();
}
