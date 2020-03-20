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

uint8_t Buffer_push_msg(uint8_t* index, uint8_t* length, uint8_t* sendBuf, uint8_t* recvBuf);
static void Test_critical_section(uint32_t p, uint32_t taskId);
#endif

