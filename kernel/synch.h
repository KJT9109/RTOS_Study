
#ifndef KERNEL_SYNCH_H_
#define KERNEL_SYNCH_H_

void Kernel_sem_init(int32_t max);
bool Kernel_sem_check(void);
void Kernel_sem_release(void);

#endif