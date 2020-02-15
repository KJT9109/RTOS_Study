/*
 * Timer.c
 *
 *  Created on: Sep 27, 2018
 *      Author: yiman
 */

#include "stdint.h"
#include "Timer.h"
#include "HalTimer.h"
#include "HalInterrupt.h"

extern volatile Timer_t* Timer;

static void interrupt_handler(void);

static uint32_t internal_1ms_counter;

void Hal_timer_init(void)
{
    // inerface reset
    Timer->timerxcontrol.bits.TimerEn = 0;   // timer mode disable
    Timer->timerxcontrol.bits.TimerMode = 0; // free running mode
    Timer->timerxcontrol.bits.OneShot = 0;   // wrapping mode
    Timer->timerxcontrol.bits.TimerSize = 0; // 16bit counter (if ==1 32bit)
    Timer->timerxcontrol.bits.TimerPre = 0;  // devided by 1
    Timer->timerxcontrol.bits.IntEnable = 1; // timer interrupt enabled
    Timer->timerxload = 0;
    Timer->timerxvalue = 0xFFFFFFFF;

    // set periodic mode
    Timer->timerxcontrol.bits.TimerMode = TIMER_PERIOIC;
    Timer->timerxcontrol.bits.TimerSize = TIMER_32BIT_COUNTER;
    Timer->timerxcontrol.bits.OneShot = 0;
    Timer->timerxcontrol.bits.TimerPre = 0;
    Timer->timerxcontrol.bits.IntEnable = 1;

    uint32_t interval = TIMER_10HZ_INTERVAL / 100; //10Hz = 0.1s 

    Timer->timerxload = interval;
    Timer->timerxcontrol.bits.TimerEn = 1;

    internal_1ms_counter = 0;

    // Register Timer interrupt handler
    Hal_interrupt_enable(TIMER_INTERRUPT);
    Hal_interrupt_register_handler(interrupt_handler, TIMER_INTERRUPT);
}

uint32_t Hal_timer_get_1ms_counter(void)
{
    return internal_1ms_counter;
}

static void interrupt_handler(void)
{
    internal_1ms_counter++;

    Timer->timerxintclr = 1;
}
