#ifndef KERNEL_TASK_H_
#define KERNEL_TASK_H_

#include "MemoryMap.h"  // TASK_STACK_SIZE 

#define NOT_ENOUGH_TASK_NUM 0xFFFFFFFF

#define USR_TASK_STACK_SIZE 0x100000     //1MB
#define MAX_TASK_NUM       (TASK_STACK_SIZE / USR_TASK_STACK_SIZE)    

// TASK_STACK_SIZE = (GLOBAL_ADDR_START - TASK_STACK_START) = 0x04800000 - 0x00800000


typedef struct KernelTaskContext_t
{
    uint32_t spsr;
    uint32_t r0_r12[13];
    uint32_t pc;
} KernelTaskContext_t;

typedef struct KernelTcb_t
{
    uint32_t sp;
    uint8_t* stack_base;
    uint32_t priority;
    uint32_t TaskId;
    
}KernelTcb_t;


typedef void (*KernelTaskFunc_t) (void);

void     Kernel_task_init(void);
void     Kernel_task_start(void);
uint32_t Kernel_task_create(KernelTaskFunc_t startFunc, uint32_t priority);
void     Kernel_task_scheduler(void);
void     Kernel_task_context_switching(void);
uint32_t Kernel_task_get_current_task_id(void);

#endif /* KERNEL_TASK_H_ */
