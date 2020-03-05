#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "task.h"

static KernelTcb_t sTask_list[MAX_TASK_NUM];
static uint32_t sAllocated_tcb_index;

void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;

    for(uint32_t i = 0; i<MAX_TASK_NUM; i++)
    {
        sTask_list[i].stack_base = (uint8_t*)(TASK_STACK_START + (i*USR_TASK_STACK_SIZE));         //STACK Base calculate
        sTask_list[i].sp         = (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE - 4;   //STACK allocate

        sTask_list[i].sp -= sizeof(KernelTaskContext_t);                         // size of Context
        KernelTaskContext_t* ctx = (KernelTaskContext_t*)sTask_list[i].sp;       // stack에 Context 저장
        ctx ->pc = 0;
        ctx ->spsr = ARM_MODE_BIT_SYS;
    }
}


uint32_t Kernel_task_create(KernelTaskFunc_t startFunc)
{
    KernelTcb_t* new_tcb = &sTask_list[sAllocated_tcb_index++];

    if (sAllocated_tcb_index > MAX_TASK_NUM)
    {
        return NOT_ENOUGH_TASK_NUM;
    }

    KernelTaskContext_t* ctx = (KernelTaskContext_t*)new_tcb->sp;
    ctx->pc = (uint32_t)startFunc;

    return (sAllocated_tcb_index - 1);   //29line에서 ++해줬으므로 다시 -1 해준다.
}