#include "stdint.h"
#include "stdbool.h"

#include "synch.h"

#define DEF_SEM_MAX 8

static int32_t sSemMax;
static int32_t sSem;

void Kernel_sem_init(int32_t max)
{
    sSemMax = (max <= 0) ? DEF_SEM_MAX : max; //(max <= 0) 이 참이면 sSemMax에 DEF_SEM_MAX을 할당 아니면 max를 할당
    sSem = sSemMax;
}

bool Kernel_sem_check(void)
{
    if (sSem <= 0)
    {
        return false;
    }

    sSem--;

    return true;
}

void Kernel_sem_release(void)
{
    if (sSem >= sSemMax)
    {
        sSem = sSemMax;
    }

    sSem++;
}

