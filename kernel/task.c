#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#include "ARMv7AR.h"
#include "task.h"

static KernelTcb_t sTask_list[MAX_TASK_NUM];
static KernelTcb_t* sCurrent_tcb;
static KernelTcb_t* sNext_tcb;
static uint32_t sAllocated_tcb_index;
static uint32_t sCurrent_tcb_index;
static uint32_t Compare_tcb_index;

static KernelTcb_t* Scheduler_round_robin_algorithm(void);
static KernelTcb_t* Scheduler_priority_algorithm(void);
static void Save_context(void);
static void Restore_context(void);


void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;
    sCurrent_tcb_index = 0;
    Compare_tcb_index = 0;

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


uint32_t Kernel_task_create(KernelTaskFunc_t startFunc, uint32_t priority)
{
    KernelTcb_t* new_tcb = &sTask_list[sAllocated_tcb_index++];

    if (sAllocated_tcb_index > MAX_TASK_NUM)
    {
        return NOT_ENOUGH_TASK_NUM;
    }

    new_tcb->priority = priority;
    new_tcb->TaskId = sAllocated_tcb_index -1;

    KernelTaskContext_t* ctx = (KernelTaskContext_t*)new_tcb->sp;
    ctx->pc = (uint32_t)startFunc;

    return (sAllocated_tcb_index - 1);   //29line에서 ++해줬으므로 다시 -1 해준다.
}

static KernelTcb_t* Scheduler_round_robin_algorithm(void)
{
    sCurrent_tcb_index++;
    sCurrent_tcb_index %= sAllocated_tcb_index;

    return &sTask_list[sCurrent_tcb_index];
}

static KernelTcb_t* Scheduler_priority_algorithm(void)
{
    
    KernelTcb_t* pCurrentTcb = &sTask_list[0];
    
    for(uint32_t i = 1; i < sAllocated_tcb_index; i++)
    {
        KernelTcb_t* PrepareTcb = &sTask_list[i];
        
        if(pCurrentTcb != PrepareTcb)
        {   
            if(pCurrentTcb -> priority > PrepareTcb -> priority)
            {
                pCurrentTcb = PrepareTcb;
            }

        }
        else
        {
            putstr("Task Compare fail#01");
            while(true);
        }
    }
    
    KernelTcb_t*  PrepareTcb = &sTask_list[Compare_tcb_index++];
    Compare_tcb_index %= sAllocated_tcb_index;

    if(pCurrentTcb -> priority == PrepareTcb -> priority)
    {
        pCurrentTcb = PrepareTcb;
    }

    sCurrent_tcb_index = (pCurrentTcb -> TaskId);
    return pCurrentTcb;
}


void Kernel_task_scheduler(void)
{
    sCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    sNext_tcb = Scheduler_priority_algorithm();

    Kernel_task_context_switching();
}

__attribute__ ((naked)) void Kernel_task_context_switching(void)
{
    __asm__ ("B Save_context");
    __asm__ ("B Restore_context");
}

uint32_t Kernel_task_get_current_task_id(void)
{
    return sCurrent_tcb_index;
}


void Kernel_task_start(void)
{
    sNext_tcb = &sTask_list[sCurrent_tcb_index];
    Restore_context();
}

static __attribute__ ((naked)) void Save_context(void)
{
    // save current task context into the current task stack
    __asm__ ("PUSH {lr}");  //__attribute__ 가 없으면 lr 이 현재 Task의 lr 이 아닌 Save_context의 lr이 들어가게 된다.
    __asm__ ("PUSH {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__ ("MRS   r0, cpsr");  //current program status register
    __asm__ ("PUSH {r0}");
    // save current task stack pointer into the current TCB
    __asm__ ("LDR   r0, =sCurrent_tcb");  //r0에 sCurrent_tcb의 값을 복사한다.
    __asm__ ("LDR   r0, [r0]");    //r0에 sCurrent_tcb의 Stack pointer 값을 복사한다.
    __asm__ ("STMIA r0, {sp}");    //sCurrent_tcb의 stack ponter 값을 갱신한다.
}

static __attribute__ ((naked)) void Restore_context(void)
{
    // restore next task stack pointer from the next TCB
    __asm__ ("LDR   r0, =sNext_tcb"); // 다음에 실행할 Task의 tcb를 r0에 load한다.
    __asm__ ("LDR   r0, [r0]");  //r0의 stack pointer를 load한다.
    __asm__ ("LDMIA r0!, {sp}"); //불러 온 stack pointer를 범용 sp에 load 한다.
    // restore next task context from the next task stack
    __asm__ ("POP  {r0}");  // sNext_tcb에 저장 된 r0(cpsr)을 복구 한다.
    __asm__ ("MSR   cpsr, r0"); // cpsr을 복구 한다.
    __asm__ ("POP  {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}"); //r0 ~ r12 까지 register를 복구한다.
    __asm__ ("POP  {pc}");  //pc register를 복구한다.
}

