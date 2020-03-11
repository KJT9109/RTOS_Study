#ifndef BOOT_MAIN_H__
#define BOOT_MAIN_H__

static void Hw_init(void);
static void printf_test(void);
static void Timer_test(void);

static void Kernel_init(void);

void User_task0(void);
void User_task1(void);
void User_task2(void);
void User_task3(void);
void User_task4(void);

#endif

