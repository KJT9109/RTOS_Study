#include "armcpu.h"

void enable_irq(void)
{
    __asm__ ("PUSH {R0,R1}");
    __asm__ ("MRS  R0, cpsr");
    __asm__ ("BIC  R1, R0, #0x80");
    __asm__ ("MSR  cpsr, R1");
    __asm__ ("POP  {R0, R1}");
}

void enable_fiq(void)
{
    __asm__ ("PUSH {R0,R1}");
    __asm__ ("MRS  R0, cpsr");
    __asm__ ("BIC  R1, R0, #0x40");
    __asm__ ("MSR  cpsr, R1");
    __asm__ ("POP  {R0, R1}");
}

void disable_irq(void)
{
    __asm__ ("PUSH {R0,R1}");
    __asm__ ("MRS  R0, cpsr");
    __asm__ ("ORR  R1, R0, #0x80");
    __asm__ ("MSR  cpsr, R1");
    __asm__ ("POP  {R0, R1}");
}

void disable_fiq(void)
{
    __asm__ ("PUSH {R0,R1}");
    __asm__ ("MRS  R0, cpsr");
    __asm__ ("ORR  R1, R0, #0x40");
    __asm__ ("MSR  cpsr, R1");
    __asm__ ("POP  {R0, R1}");
}

